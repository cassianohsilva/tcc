#ifndef MESH_H
#define MESH_H

#include "../Array/bag.h"
#include "../vectors.h"
#include "../../MultiLevelArray.h"
#include "../matrix.h"

#include "edge.h"
#include "vertex.h"
#include "triangle.h"
#include "vertex_parent.h"

class MeshDrawer;

// ======================================================================
// ======================================================================

class Mesh {

public:

	// ========================
	// CONSTRUCTOR & DESTRUCTOR
	Mesh();
	virtual ~Mesh();

	// ========
	// VERTICES
	std::size_t numVertices() const {
		return vertices->size();
	}
	Vertex* addVertex(const Vec3f &pos, std::size_t level);

	void removeVertex(Vertex *vertex);
	// this creates a relationship between 3 vertices (2 parents, 1 child)
	void setParentsChild(Vertex *p1, Vertex *p2, Vertex *child);

	// Delete the relation between parents and child and return child
	Vertex* deleteParentsChildRelation(Vertex *p1, Vertex *p2);

	// this accessor will find a child vertex (if it exists) when given
	// two parent vertices
	Vertex* getChildVertex(Vertex *p1, Vertex *p2) const;
	// look up vertex by index from original .obj file

	// =====
	// EDGES
	std::size_t numEdges() const {
		return edges->count();
	}
	// this efficiently looks for an edge with the given vertices, using a hash table
	Edge* getEdge(Vertex *a, Vertex *b) const;

	// =========
	// TRIANGLES
	std::size_t numTriangles() const {
		return triangles->count();
	}
	Triangle* addTriangle(Vertex *a, Vertex *b, Vertex *c);
	void removeTriangle(Triangle *t);

	void printTrianglesPointers(std::size_t limit = 1000);

	Vec3f computeNormal(const Vec3f &p1, const Vec3f &p2, const Vec3f &p3);

	void printTriangles(std::size_t limit = 1000);
	void printVertices(std::size_t limit = 1000);

	virtual void reset() {
	}

	virtual Matrix getTransform() {
		return Matrix::identity();
	}

	void updateNormals();

	friend class CelestialBody;
	friend class MeshDrawer;

protected:

	// ==============
	// REPRESENTATION
	MultiLevelArray<Vertex*> *vertices;

	Bag<Edge*> *edges;
	Bag<Triangle*> *triangles;
	Bag<VertexParent*> *vertex_parents;

	friend class QuadtreeMesh;
};

// ======================================================================
// ======================================================================

#endif

