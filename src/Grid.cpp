#include "Grid.h"

Grid::Grid(Eigen::Vector2f pos, Eigen::Vector2f dims, Eigen::Vector2f cells, SnowObject* object){
	obj = object;
	origin = pos;
	cellsize << dims(0) / cells(0), dims(1) / cells(1);
	size << cells(0) + 1, cells(1) + 1;
	nodes_length = size.prod();
	nodes = new GridNode[nodes_length];
	clear_nodes();
}

void Grid::clear_nodes() {
	memset(nodes, 0, sizeof(GridNode) * nodes_length);
}

void Grid::rasterize_particle_data() {
	transfer_mass();
	transfer_velocity();
}

void Grid::transfer_mass() {
	for (int i = 0; i < obj->size; i++) {
		Particle& p = obj->particles[i];

		Eigen::Vector2f p_scaled; //Xp/h
		get_gird_pos(p_scaled, p.position);

		int base_x = floorf(p_scaled(0));
		int base_y = floorf(p_scaled(1));

		int idx_for_weights = 0;

		for (int x = base_x - MIN; x <= base_x + MAX; x++) {
			for (int y = base_y - MIN; y <= base_y + MAX; y++) {
				if (!(y < 0 || y > size(1) || x < 0 || x > size(0))) {
					float Nx = b_spline(p_scaled(0) - x);
					float dNx = D_spline_dx(p_scaled(0) - x);
					float Ny = b_spline(p_scaled(1) - y);
					float dNy = D_spline_dx(p_scaled(1) - y);

					Eigen::Vector2f dw(dNx * Ny / cellsize(0), Nx * dNy / cellsize(1));
					p.weights[idx_for_weights] = Ny * Nx;
					p.d_weight[idx_for_weights] = dw;

					float weight = p.mass * p.weights[idx_for_weights];
					nodes[(int)(y * size[0] + x)].mass += weight;
				}
				idx_for_weights++;
			}
		}
	}
}

void Grid::transfer_velocity() {
	for (int i = 0; i < obj->size; i++) {
		Particle& p = obj->particles[i];

		Eigen::Vector2f p_scaled; //Xp/h
		get_gird_pos(p_scaled, p.position);

		int base_x = floorf(p_scaled(0));
		int base_y = floorf(p_scaled(1));

		int idx_for_weights = 0;
		
		for (int x = base_x - MIN; x <= base_x + MAX; x++) {
			for (int y = base_y - MIN; y <= base_y + MAX; y++) {
				if (!(y < 0 || y > size(1) || x < 0 || x > size(0))) {
					float w = p.weights[idx_for_weights];
					float weight = p.mass * w;
					int idx = (int)(y * size[0] + x);
					nodes[idx].velocity += p.velocity * weight / nodes[idx].mass;
				}
				idx_for_weights++;
			}
		}
	}
}

void Grid::compute_particle_volumes() {
	for (int i = 0; i < obj->size; i++) {
		Particle& p = obj->particles[i];

		Eigen::Vector2f p_scaled; //Xp/h
		get_gird_pos(p_scaled, p.position);

		int base_x = floorf(p_scaled(0));
		int base_y = floorf(p_scaled(1));

		p.density = 0;
		int idx_for_weights = 0;

		for (int x = base_x - MIN; x <= base_x + MAX; x++) {
			for (int y = base_y - MIN; y <= base_y + MAX; y++) {
				if (!(y < 0 || y > size(1) || x < 0 || x > size(0))) {
					float w = p.weights[idx_for_weights];
					if (w > BSPLINE_EPSILON) {
						p.density += w * nodes[(int)(y * size[0] + x)].mass;
					}
				}
				idx_for_weights++;
			}
		}

		p.density /= cellsize.prod();
		p.volume = p.mass / p.density;
	}
}

void Grid::get_gird_pos(Eigen::Vector2f &pos, Eigen::Vector2f partical_pos) {
	pos = partical_pos - origin;
	pos << pos(0) / cellsize(0), pos(1) / cellsize(1);
}

void Grid::get_node_pos(Eigen::Vector2f& p, int i, int j) {
	p = origin + Eigen::Vector2f(cellsize(0) * i, cellsize(1) * j);
}

void Grid::compute_internal_force() {
	for (int i = 0; i < obj->size; i++) {
		Particle& p = obj->particles[i];
		Eigen::Matrix2f stress_force;
		p.compute_stress_based_force(stress_force);

		Eigen::Vector2f p_scaled; //Xp/h
		get_gird_pos(p_scaled, p.position);

		int base_x = floorf(p_scaled(0));
		int base_y = floorf(p_scaled(1));

		int idx_for_weights = 0;

		for (int x = base_x - MIN; x <= base_x + MAX; x++) {
			for (int y = base_y - MIN; y <= base_y + MAX; y++) {
				if (!(y < 0 || y > size(1) || x < 0 || x > size(0))) {
					float w = p.weights[idx_for_weights];
					if (w > BSPLINE_EPSILON) {
						nodes[(int)(y * size[0] + x)].force -= stress_force * p.d_weight[idx_for_weights];
					}
				}
				idx_for_weights++;
			}
		}
	}
}

void Grid::update_velocity() {
	compute_internal_force();
	for (int i = 0; i < nodes_length; i++) {
		if (nodes[i].mass != 0) {
			nodes[i].next_velocity = nodes[i].velocity + TIMESTEP * (GRAVITY + nodes[i].force / nodes[i].mass);
		}
		
	}
}

void Grid::update_particle_velocity() {	
	for (int i = 0; i < obj->size; i++) {
		Particle& p = obj->particles[i];
		Eigen::Matrix2f& dv = p.d_velocity;
		dv.setZero();
		
		Eigen::Vector2f p_scaled, pic, flip;
		pic.setZero();
		flip << p.velocity;
		get_gird_pos(p_scaled, p.position);

		int base_x = floorf(p_scaled(0));
		int base_y = floorf(p_scaled(1));

		int idx_for_weights = 0;

		for (int x = base_x - MIN; x <= base_x + MAX; x++) {
			for (int y = base_y - MIN; y <= base_y + MAX; y++) {
				if (!(y < 0 || y > size(1) || x < 0 || x > size(0))) {
					float w =p.weights[idx_for_weights];
					if (w > BSPLINE_EPSILON) {
						int idx = (int)(y * size[0] + x);
						Eigen::Vector2f dw = p.d_weight[idx_for_weights];

						pic += nodes[idx].next_velocity * w;
						flip += (nodes[idx].next_velocity - nodes[idx].velocity) * w;
						dv += nodes[idx].next_velocity * dw.transpose();
					}
				}
				idx_for_weights++;
			}
		}
		p.velocity = FLIP_PERCENT * flip + (1 - FLIP_PERCENT) * pic;
	}
}

/////// handle collision, only handles wall collision rght now //////////////

void Grid::handle_collision(Eigen::Vector2f& pos, Eigen::Vector2f& velocity) {
	// wall collision only, no velocity.
	Eigen::Vector2f coll_vel(0, 0);
	Eigen::Vector2f n(0, 0);
	Eigen::Vector2f position;
	bool verticle_surface = false;
	get_gird_pos(position, pos);


	if (position[0] < BSPLINE_RADIUS) {
		verticle_surface = true;
		n(0) = 1;
	}
	else if (position[0] > size[0] - BSPLINE_RADIUS - 1) {
		verticle_surface = true;
		n(0) = -1;
	}
	else if (position[1] < BSPLINE_RADIUS) {
		n(1) = 1;
	}
	else if (position[1] > size[1] - BSPLINE_RADIUS - 1) {
		n(1) = -1;
	}
	else {
		return;
	}

	n.normalize();

	Eigen::Vector2f v_rel = velocity - coll_vel;
	float vn = v_rel.dot(n);
	if (vn < 0) {
		Eigen::Vector2f v_t = v_rel - vn * n;
		if (v_t.norm() <= -1.0 * MU * vn || verticle_surface) {
			v_rel.setZero();
		}
		else {
			v_rel = v_t + MU * vn * v_t.normalized();
		}

		velocity = v_rel + coll_vel;
	}
}

void Grid::handle_particle_collision() {
	for (int i = 0; i < obj->size; i++) {
		Particle& p = obj->particles[i];
		handle_collision(p.position, p.velocity);
	}
}

void Grid::handle_grid_collision() {
	for (int y = 0; y < size[1]; y++) {
		for (int x = 0; x < size[0]; x++) {
			int idx = (int)(y * size[0] + x);
			GridNode& node = nodes[idx];
			Eigen::Vector2f pos;
			get_node_pos(pos, x, y);
			handle_collision(pos, node.next_velocity);
		}
	}
}

void Grid::draw() {

	if (SUPPORTS_POINT_SMOOTH) glDisable(GL_POINT_SMOOTH);

	glPointSize(1);
	glColor3f(.2, .2, .2);

	glBegin(GL_POINTS);
	for (int y = 0; y < size[1]; y++) {
		for (int x = 0; x < size[0]; x++) {
			Eigen::Vector2f pos;
			get_node_pos(pos, x, y);
			glVertex2f(pos[0], pos[1]);
		}
	}

	glEnd();
}