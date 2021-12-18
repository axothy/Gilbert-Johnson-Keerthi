#include "GJK_Algorithm.hpp"

int main(void)
{
	std::vector<Figure*> figures;
	Reader readfile(figures);

	CollisionDetector collisions(figures);
	collisions.showCollisions();

	return 0;
}