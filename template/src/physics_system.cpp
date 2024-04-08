// internal
#include "physics_system.hpp"
#include "world_init.hpp"
#include "common.hpp"
#include <stack>

// Returns the local bounding coordinates scaled by the current size of the entity
vec2 get_bounding_box(const Motion& motion)
{
	// abs is to avoid negative scale due to the facing direction.
	return { abs(motion.scale.x), abs(motion.scale.y) };
}

// code for graham scan including utilities below are refactorings from
// https://www.geeksforgeeks.org/convex-hull-using-graham-scan/

// A global point needed for  sorting points with reference
// to  the first point Used in compare function of qsort()
vec2 p0;

// A utility function to find next to top in a stack
vec2 nextToTop(std::stack<vec2>& S)
{
	vec2 p = S.top();
	S.pop();
	vec2 res = S.top();
	S.push(p);
	return res;
}

// A utility function to swap two points
void swap(vec2& p1, vec2& p2)
{
	vec2 temp = p1;
	p1 = p2;
	p2 = temp;
}

// A utility function to return square of distance
// between p1 and p2
int distSq(vec2 p1, vec2 p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) +
		(p1.y - p2.y) * (p1.y - p2.y);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are collinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(vec2 p, vec2 q, vec2 r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // collinear
	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// A function used by library function qsort() to sort an array of
// points with respect to the first point
int compare(const void* vp1, const void* vp2)
{
	vec2* p1 = (vec2*)vp1;
	vec2* p2 = (vec2*)vp2;

	// Find orientation
	int o = orientation(p0, *p1, *p2);
	if (o == 0)
		return (distSq(p0, *p2) >= distSq(p0, *p1)) ? -1 : 1;

	return (o == 2) ? -1 : 1;
}

// generates a convex hull using graham scan that returns the ordered list of points 
// of the convex hull in a clockwise manner
std::vector<vec2> graham_scan(std::vector<vec2> points, int n) {
	// Find the bottommost point
	int ymin = points[0].y, min = 0;
	for (int i = 1; i < n; i++)
	{
		int y = points[i].y;

		// Pick the bottom-most or choose the left
		// most point in case of tie
		if ((y < ymin) || (ymin == y &&
			points[i].x < points[min].x))
			ymin = points[i].y, min = i;
	}

	// Place the bottom-most point at first position
	swap(points[0], points[min]);

	// Sort n-1 points with respect to the first point.
	// A point p1 comes before p2 in sorted output if p2
	// has larger polar angle (in counterclockwise
	// direction) than p1
	p0 = points[0];
	qsort(&points[1], n - 1, sizeof(vec2), compare);

	// If two or more points make same angle with p0,
	// Remove all but the one that is farthest from p0
	// Remember that, in above sorting, our criteria was
	// to keep the farthest point at the end when more than
	// one points have same angle.
	int m = 1; // Initialize size of modified array
	for (int i = 1; i < n; i++)
	{
		// Keep removing i while angle of i and i+1 is same
		// with respect to p0
		while (i < n - 1 && orientation(p0, points[i],
			points[i + 1]) == 0)
			i++;


		points[m] = points[i];
		m++;  // Update size of modified array
	}

	// If modified array of points has less than 3 points,
	// convex hull is not possible
	if (m < 3) return std::vector<vec2>();

	// Create an empty stack and push first three points
	// to it.
	std::stack<vec2> S;
	S.push(points[0]);
	S.push(points[1]);
	S.push(points[2]);

	// Process remaining n-3 points
	for (int i = 3; i < m; i++)
	{
		// Keep removing top while the angle formed by
		// points next-to-top, top, and points[i] makes
		// a non-left turn
		while (S.size() > 1 && orientation(nextToTop(S), S.top(), points[i]) != 2)
			S.pop();
		S.push(points[i]);
	}

	// create output vector
	std::vector<vec2> convex_hull_points;
	while (!S.empty()) {
		convex_hull_points.push_back(S.top());
		S.pop();
	}
	return convex_hull_points;
}

bool onSegment(vec2 p, vec2 q, vec2 r)
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;

	return false;
}

// Function to check if two line segments intersect
bool do_intersect(vec2 p1, vec2 q1, vec2 p2, vec2 q2) {
	// Find orientations
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	/*if ((o1 * o2 < 0) && (o3 * o4 < 0))
		return true;*/
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases 
	// p1, q1 and p2 are collinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and q2 are collinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are collinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are collinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases
}

bool is_inside_bounding_box(vec2 p, float xmin, float xmax, float ymin, float ymax) {
	return p.x >= xmin && p.x <= xmax && p.y >= ymin && p.y <= ymax;
}

bool convex_hull_inside_box(std::vector<vec2> convexHull, float xmin, float xmax, float ymin, float ymax) {
	for (const vec2& p : convexHull) {
		if (!is_inside_bounding_box(p, xmin, xmax, ymin, ymax)) {
			return false; // If any point is outside, return false
		}
	}
	return true; // All points are inside the bounding box
}

// Function to check collision between convex hull and a bounding box
bool is_collision(std::vector<vec2>& convexHull, float xmin, float xmax, float ymin, float ymax) {
	int n = convexHull.size();
	for (int i = 0; i < n; i++) {
		vec2 p1 = convexHull[i];
		vec2 p2 = convexHull[(i + 1) % n];
		if (do_intersect(p1, p2, { xmin, ymin }, { xmax, ymin }) ||
			do_intersect(p1, p2, { xmax, ymin }, { xmax, ymax }) ||
			do_intersect(p1, p2, { xmax, ymax }, { xmin, ymax }) ||
			do_intersect(p1, p2, { xmin, ymax }, { xmin, ymin })) {
			return true;
		}
	}
	// just need to check if convex hull is entirely inside box
	return convex_hull_inside_box(convexHull, xmin, xmax, ymin, ymax);
}

// for debugging each side of convex hull
Entity createLine_points(vec2 p1, vec2 p2) {
	vec2 center = { (p1.x + p2.x) / 2, (p1.y + p2.y) / 2 };

	// Calculate scale
	float distance = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
	vec2 scale = { distance, 5 }; // Assuming uniform scaling

	// Calculate angle
	float angle = atan2(p2.y - p1.y, p2.x - p1.x);

	return createLine(center, scale, angle);
}

// Mesh-box collision detection, using graham scan for approximate convex hull for players
bool player_mesh_collides(const Entity& player, const Entity& box) {
	//get box corner points
	Motion& box_motion = registry.motions.get(box);
	float xmin = box_motion.position.x - box_motion.scale.x / 2;
	float xmax = xmin + box_motion.scale.x;
	float ymin = box_motion.position.y - box_motion.scale.y / 2;
	float ymax = ymin + box_motion.scale.y;
	if (debugging.in_debug_mode) {
		createLine_points({ xmin, ymin }, { xmin,ymax });
		createLine_points({ xmin,ymin }, { xmax, ymin });
		createLine_points({ xmin,ymax }, { xmax,ymax });
		createLine_points({ xmax,ymin }, { xmax,ymax });
	}
	//compute approximate convex hull using players vertexes
	Motion player_motion = registry.motions.get(player);
	Transform player_transform;
	player_transform.translate(player_motion.position);
	player_transform.scale(player_motion.scale);
	player_transform.rotate(-player_motion.angle);
	std::vector<vec2> points;
	Mesh* player_mesh = registry.meshPtrs.get(player);
	for (ColoredVertex const cv : player_mesh->vertices) {
		vec3 pos = player_transform.mat * vec3(cv.position.x, cv.position.y, 1.0f);
		points.push_back({ pos.x, pos.y });
	}

	int n_convex_hull = points.size();
	std::vector<vec2> convex_hull = graham_scan(points, n_convex_hull);

	// TODO: check if the convex hull is correct using createLines
	if (debugging.in_debug_mode) {
		int n = convex_hull.size();
		for (int i = 0; i < n; i++) {
			vec2 point1 = convex_hull[i];
			vec2 point2 = convex_hull[(i + 1) % n];
			createLine_points(point1, point2);
			//createLine({ point1.x,point1.y }, { 5,5 });
		}
	}

	// based on the convex hull, check for intersections, and if all points in the convex hull is inside the box
	if (is_collision(convex_hull, xmin, xmax, ymin, ymax)) {
		return true;
	}
	return false;
}

// This is a SUPER APPROXIMATE check that puts a circle around the bounding boxes and sees
// if the center point of either object is inside the other's bounding-box-circle. You can
// surely implement a more accurate detection
// REQUIRES: entities passed must have a motion
bool collides(const Entity& entity1, const Entity& entity2)
{
	if (registry.players.has(entity1) && !registry.players.has(entity2)) {
		return player_mesh_collides(entity1, entity2);
	}
	else if (registry.players.has(entity2) && !registry.players.has(entity1)) {
		return player_mesh_collides(entity2, entity1);
	}
	else {
		// box-box collision
		Motion motion1 = registry.motions.get(entity1);
		Motion motion2 = registry.motions.get(entity2);

		vec2 dp = motion1.position - motion2.position;
		float dist_squared = dot(dp, dp);
		const vec2 other_bonding_box = get_bounding_box(motion1) / 2.f;
		const float other_r_squared = dot(other_bonding_box, other_bonding_box);
		const vec2 my_bonding_box = get_bounding_box(motion2) / 2.f;
		const float my_r_squared = dot(my_bonding_box, my_bonding_box);
		const float r_squared = max(other_r_squared, my_r_squared);


		if (dist_squared < r_squared) {
			return true;
		}
	}
	return false;
}

void PhysicsSystem::step(float elapsed_ms)
{
	// Move bug based on how much time has passed, this is to (partially) avoid
	// having entities move at different speed based on the machine.
	auto& motion_registry = registry.motions;
	for(uint i = 0; i< motion_registry.size(); i++)
	{
		Motion& motion = motion_registry.components[i];
		Entity entity = motion_registry.entities[i];
		if (registry.debugComponents.has(entity)) {
			continue;
		}
		float step_seconds = elapsed_ms / 1000.f;

		if (registry.players.has(entity)) {
			float x_pos = motion.position.x + (motion.velocity.x * step_seconds);
			float y_pos = motion.position.y + (motion.velocity.y * step_seconds);
			// need to make a variable for the player size once mesh is done
			if (x_pos <= 20.0 || x_pos >= window_width_px - 20.0) motion.velocity.x = 0.f;
			if (y_pos <= BG_HEIGHT || y_pos >= window_height_px - 20.0) motion.velocity.y = 0.f;
		}

		if (registry.enemyDrinks.has(entity)) {
			float y_pos = motion.position.y + (motion.velocity.y * step_seconds);
			if (y_pos <= BG_HEIGHT) motion.velocity.y *= -1.f;
		}

		// adding acceleration to sparkle
		if (registry.sparkles.has(entity)) {
			Sparkle& sparkle = registry.sparkles.get(entity);
			motion.velocity += sparkle.acceleration;
			motion.angle += M_PI / 12.f;
			//motion.position.x = sin(step_seconds * 2.f * M_PI);
		}
		motion.position += motion.velocity * step_seconds;
		// (void)elapsed_ms; // placeholder to silence unused warning until implemented
	}

	
	// Check for collisions between all moving entities
	/*
    ComponentContainer<Motion> &motion_container = registry.motions;
	for(uint i = 0; i<motion_container.components.size(); i++)
	{
		Motion& motion_i = motion_container.components[i];
		Entity entity_i = motion_container.entities[i];

		if (registry.debugComponents.has(entity_i)) {
			continue;
		}
		
		// note starting j at i+1 to compare all (i,j) pairs only once (and to not compare with itself)
		for(uint j = i+1; j<motion_container.components.size(); j++)
		{
			Motion& motion_j = motion_container.components[j];
			Entity entity_j = motion_container.entities[j];
			if (registry.debugComponents.has(entity_j)) {
				continue;
			}
			if (collides(entity_i, entity_j))
			{
				Entity entity_j = motion_container.entities[j];
				// Create a collisions event
				// We are abusing the ECS system a bit in that we potentially insert muliple collisions for the same entity
				registry.collisions.emplace_with_duplicates(entity_i, entity_j);
				registry.collisions.emplace_with_duplicates(entity_j, entity_i);
			}
			// ADDED AS PART OF THE COLLISION WITH LEVELNODES
			if (collides(entity_i, entity_j) && ((registry.players.has(entity_j) && registry.levelNode.has(entity_i)) || (registry.players.has(entity_i) && registry.levelNode.has(entity_j))))
			{
				Entity entity_j = motion_container.entities[j];
				// Create a collisions event
				// We are abusing the ECS system a bit in that we potentially insert muliple collisions for the same entity
				registry.collisions.emplace_with_duplicates(entity_i, entity_j);
				registry.collisions.emplace_with_duplicates(entity_j, entity_i);

			}
		}
	}
	*/

}