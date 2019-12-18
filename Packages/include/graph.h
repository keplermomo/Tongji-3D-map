#include<iostream>
#include<iomanip>
#include<string>
#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


/**
 * File: graph.h
 * Description: This file contains the definition and implementation of the
 *              Graph class and other functions.
 */

#ifndef _GRAPH_H_
#define _GRAPH_H_

using namespace std;
namespace GraphNameSpace
{

	struct Type {
		glm::vec3 pos;
		string name;
		Type() {}
		Type(string n, glm::vec3 p) {
			name = n;
			pos = p;
		}

		friend ostream& operator<<(ostream &out, const Type &value) {
			out << "Name:< " << value.name << " > : [ " << value.pos.x << "," << value.pos.y << "," << value.pos.z << " ] ";
			return out;
		}
		Type& operator=(const Type& value)
		{
			pos = value.pos;
			name = value.name;
			return *this;
		}
		bool operator==(const Type &p)
		{
			return(pos == p.pos && name == p.name);
		}
		
	};
	
  /**
  * Description: A struct representing properties of a edges including 
  * its weight and the associated vertices
  */
    
    struct ConnectedVertices
    {
      int connIndex; // index of the vertex on the other end of the edge
      int edgeWeight; // weight if its a weighted graph
	  ConnectedVertices& operator=(const   ConnectedVertices& value)
	  {
		  connIndex = value.connIndex;
		  edgeWeight = value.edgeWeight;
		  return *this;
	  }
    };
  /**
  * Description: A struct representing a vertex
  */  
    
    struct Vertex
    {
      Type info;	 // info holded by the vertex
      int vertexIndex; // position in the graph
      int countAdj;	// number of adjacent vertices to this vertex
      ConnectedVertices edge[100]; // and array of adjuscent vertices

	  Vertex& operator=(const  Vertex& value)
	  {
		  info = value.info;
		  vertexIndex = value.vertexIndex;
		  countAdj = value.countAdj;
		  for (int i = 0; i < 100; i++)
			  edge[i] = value.edge[i];
		  return *this;
	  }
    };
    
    enum Weight{WEIGHTED, UNWEIGHTED};
    enum Direction{DIRECTED, UNDIRECTED};
    
    
    class Graph
    {
    public:
      
      /**
      * Function: Graph - The default constructor.
      * Description: Constructs a new Graph object with no arguments passed.
      * Function input: None.
      * Function output: None.
      * Precondition: none.
      * Postcondition: Graph object created with no size
      */
      Graph();
      
    /**
      * Function: Array - The overloaded constructor with direction and weight
      * Description: Constructs a new Graph object with argument passed to it.
      * Function input: direction and weight of the graph
      * Function output: None.
      * Precondition: none.
      * Postcondition: Graph object created and initialized with
      *                the size passed
      */
      Graph(Direction, Weight);

    /**
      * Function: Array - The overloaded constructor with direction and weight
      * Description: Constructs a new Graph object with argument passed to it.
      * Function input: direction and weight of the graph
      * Function output: None.
      * Precondition: none.
      * Postcondition: Graph object created and initialized with
      *                the size passed
      */
      Graph(Weight, Direction);
      
    /**
      * Function: Array - The overloaded constructor with direction
      * Description: Constructs a new Graph object with argument passed to it.
      * Function input: direction of the graph
      * Function output: None.
      * Precondition: none.
      * Postcondition: Graph object created and initialized with
      *                the size passed
      */
      Graph(Direction);
      
    /**
      * Function: Array - The overloaded constructor with weight
      * Description: Constructs a new Graph object with argument passed to it.
      * Function input: weight of the graph
      * Function output: None.
      * Precondition: none.
      * Postcondition: Graph object created and initialized with
      *                the size passed
      */
      Graph(Weight);
      
    /**
      * the copy constructor
      */
      Graph(const Graph& otherGraph);
      
      /**
      * overloading the assignment operator
      */
      const Graph& operator=(const Graph& arg);
      
    /**
      * Function: ~Graph -The destructor
      * Description: detroys the object when it goes out of scope
      * Function input: none
      * Function output: None.
      * Precondition: graph should exist.
      * Postcondition: Graph object is destroyed to prevent memory leak
      */
      ~Graph();
      
    /**
      * Function: destroy 
      * Description: destroy the object when it goes out of scope
      * Function input: none
      * Function output: None.
      * Precondition: graph should exist.
      * Postcondition: Graph object is destroyed to prevent memory leak
      */
      void destroy();
      
    /**
      * Function: isEmpty
      * Description: checks if the graph is empty
      * Function input: none
      * Function output: None.
      * Precondition: graph should exist.
      * Postcondition: returns true is graph is empty or false otherwise
      */
      bool isEmpty() const; // no vertices
      
    /**
      * Function: isFull
      * Description: checks if the graph is full
      * Function input: none
      * Function output: None.
      * Precondition: graph should exist.
      * Postcondition: returns true if graph is full or false otherwise
      */
      bool isFull() const;
      
    /**
      * Function: isAdjacentTo
      * Description: checks if there is an edge between two vertices
      * Function input: two vertices
      * Function output: None.
      * Precondition: edges should exist
      * Postcondition: returns true if adjancency exist or false otherwise
      */
      bool isAdjacentTo(const Type&,const Type&) const throw (std::logic_error);

    /**
      * Function: edgeWeight
      * Description: returns the weight of the edge between 2 vertices
      * Function input: two vertices
      * Function output: the weight of the edge
      * Precondition: edges should exist
      * Postcondition: the weight of the edge is returned
      */
      int edgeWeight(const Type&,const Type&) const throw (std::logic_error);// precondition: edge exists
      
      /**
      * Function: edgeWeight
      * Description: returns the number of edges in the graph
      * Function input: none
      * Function output: the number of edges in the graph.
      * Precondition: edges should exist
      * Postcondition: the weight of the edge is returned
      */
      int edgeCount() const;
      
      /**
      * Function: vertexCount
      * Description: returns the number of vertices in the graph
      * Function input: none
      * Function output: the number of vertices in the graph.
      * Precondition: edges should exist
      * Postcondition: the number of vertices in the graph is returned
      */
      int vertexCount() const;
      
      /**
      * Function: insertVertex
      * Description: inserts a vertex in the graph
      * Function input: a vertex
      * Function output: none
      * Precondition: a graph should exist
      * Postcondition: a vertex is inserted to the graph
      */
      void insertVertex(const Type&) throw (std::range_error, std::logic_error);
      
      /**
      * Function: insertEdge
      * Description: inserts an edge between 2 vertices
      * Function input: a 2 edges and weight if exists
      * Function output: none
      * Precondition: the vertices should exist
      * Postcondition: a edge is inserted betwen tow vertices
      */
      void insertEdge(const Type&,const Type&, int weight=1);
      
      /**
      * Function: deleteEdge
      * Description: deletes an edge between 2 vertices
      * Function input: 2 vertices
      * Function output: none
      * Precondition: an edge between the passed vertices should exist should exist
      * Postcondition: the edge is deleted betwen the two vertices
      */
      void deleteEdge(const Type&,const Type&) throw (std::logic_error);
      
    /**
      * Function: deleteVertex
      * Description: deletes a vertex
      * Function input: a vertex
      * Function output: none
      * Precondition: the vertex should exist
      * Postcondition: the vertex is deleted
      */
      void deleteVertex(const Type&) throw (std::logic_error);
      
    /**
      * Function: findVertex
      * Description: finds the position  of th evertex in the graph
      * Function input: a vertex
      * Function output: the position of the vertex
      * Precondition: the vertex should exist
      * Postcondition: the vertex position if returned
      */
      int findVertex(const Type& vertex) const;
      
    /**
      * Function: dump
      * Description: prints the whole graph
      * Function input: none
      * Function output: none
      * Precondition: a graph object should exist
      * Postcondition: the graph is printed
      */
      void dump() const;
      

	  int findRoad(const Type& vertex_dest, glm::vec3 origin);

      Weight weigh;  // is graph weighted?
      Direction direction; // is the graph directed?
      int edgeCountNum; // the numbr of edges in the graph
      int count; // the number of vertices in the graph
	  glm::vec3 *path;

    private:
      Vertex *node;  // the collection of vertices in the graph
    };
    


}
#endif
