#include "graph.h"

using namespace GraphNameSpace;




Graph::Graph()
{
	weigh = UNWEIGHTED;
	direction = UNDIRECTED;
	node = new Vertex[100];
	count = 0;
	edgeCountNum = 0;
}


Graph::Graph(Direction dir, Weight weight)
{
	weigh = weight;
	direction = dir;
	node = new Vertex[100];
	count = 0;
	edgeCountNum = 0;
}


Graph::Graph(Weight weight, Direction dir)
{
	weigh = weight;
	direction = dir;
	node = new Vertex[100];
	count = 0;
	edgeCountNum = 0;
}


Graph::Graph(Direction dir)
{
	weigh = UNWEIGHTED;
	direction = dir;
	node = new Vertex[100];
	count = 0;
	edgeCountNum = 0;
}


Graph::Graph(Weight weight)
{
	weigh = weight;
	direction = UNDIRECTED;
	node = new Vertex[100];
	path = NULL;
	count = 0;
	edgeCountNum = 0;
}


bool Graph::isEmpty() const
{
	if (count == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Graph::isFull() const
{
	if (count == 100)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Graph::insertVertex(const Type& itemToInsert) throw (std::range_error, std::logic_error)
{
	try
	{
		if (isFull() == true)
			throw std::range_error("Graph is full");

		int vertexIndexNumDelete;
		bool vertex_exists = false;

		int edgeWeightNum;

		int deleteIndex;

		for (int i = 0; i< count; i++)
		{
			if (vertex_exists == false)
			{
				if (node[i].info == itemToInsert)
				{
					vertex_exists = true;
					vertexIndexNumDelete = i;
				}
			}
		}
		if (vertex_exists == true)
			throw std::logic_error("Item already exists in the Graph and will not be inserted");

		node[count].info = itemToInsert;
		node[count].vertexIndex = count;
		node[count].countAdj = 0;
		count += 1;
	}
	catch (const std::out_of_range bad_range)
	{
		cerr << "out_of_range: " << bad_range.what() << '\n';
	}
	catch (const std::logic_error item_exists)
	{
		cerr << "logic_error: " << item_exists.what() << '\n';
	}
}


void Graph::insertEdge(const Type& fromVertex, const Type& toVertex, int weight)
{
	if (weigh == 0 && direction == 0) // if(weigh == WEIGHTED && direction == DIRECTED)
	{
		int indexFrom;
		int indexTo;

		bool from_exists = false;
		bool to_exists = false;

		for (int i = 0; i< count; i++)
		{
			if (from_exists == false)
			{
				if (node[i].info == fromVertex)
				{
					from_exists = true;
					indexFrom = i;
				}
			}
			if (to_exists == false)
			{
				if (node[i].info == toVertex)
				{
					to_exists = true;
					indexTo = i;
				}
			}
		}
		node[indexFrom].edge[node[indexFrom].countAdj].connIndex = indexTo;
		node[indexFrom].edge[node[indexFrom].countAdj].edgeWeight = weight;
		node[indexFrom].countAdj++;
	}
	else if (weigh == 1 && direction == 1) // if(weigh == UNWEIGHTED && direction == UNDIRECTED)
	{
		int indexFrom;
		int indexTo;

		bool from_exists = false;
		bool to_exists = false;

		for (int i = 0; i< count; i++)
		{
			if (from_exists == false)
			{
				if (node[i].info == fromVertex)
				{
					from_exists = true;
					indexFrom = i;
				}
			}
			if (to_exists == false)
			{
				if (node[i].info == toVertex)
				{
					to_exists = true;
					indexTo = i;
				}
			}
		}
		node[indexFrom].edge[node[indexFrom].countAdj].connIndex = indexTo;
		node[indexFrom].edge[node[indexFrom].countAdj].edgeWeight = 0;
		node[indexFrom].countAdj++;

		node[indexTo].edge[node[indexTo].countAdj].connIndex = indexFrom;
		node[indexTo].edge[node[indexTo].countAdj].edgeWeight = 0;
		node[indexTo].countAdj++;
	}
	else if (weigh == 1 && direction == 0) // if(weigh == UNWEIGHTED && direction == DIRECTED)
	{
		int indexFrom;
		int indexTo;

		bool from_exists = false;
		bool to_exists = false;

		for (int i = 0; i< count; i++)
		{
			if (from_exists == false)
			{
				if (node[i].info == fromVertex)
				{
					from_exists = true;
					indexFrom = i;
				}
			}
			if (to_exists == false)
			{
				if (node[i].info == toVertex)
				{
					to_exists = true;
					indexTo = i;
				}
			}
		}
		node[indexFrom].edge[node[indexFrom].countAdj].connIndex = indexTo;
		node[indexFrom].edge[node[indexFrom].countAdj].edgeWeight = 0;
		node[indexFrom].countAdj++;
	}
	else if (weigh == 0 && direction == 1) // if(weigh == WEIGHTED && direction == UNDIRECTED)
	{
		int indexFrom;
		int indexTo;

		bool from_exists = false;
		bool to_exists = false;

		for (int i = 0; i< count; i++)
		{
			if (from_exists == false)
			{
				if (node[i].info == fromVertex)
				{
					from_exists = true;
					indexFrom = i;
				}
			}
			if (to_exists == false)
			{
				if (node[i].info == toVertex)
				{
					to_exists = true;
					indexTo = i;
				}
			}
		}
		node[indexFrom].edge[node[indexFrom].countAdj].connIndex = indexTo;
		node[indexFrom].edge[node[indexFrom].countAdj].edgeWeight = weight;
		node[indexFrom].countAdj++;

		node[indexTo].edge[node[indexTo].countAdj].connIndex = indexFrom;
		node[indexTo].edge[node[indexTo].countAdj].edgeWeight = weight;
		node[indexTo].countAdj++;
	}
	edgeCountNum += 1;
}


void Graph::dump() const
{
	string type;
	string weight;
	//     
	if (weigh == 0)
		weight = "WEIGHTED";
	else
		weight = "UNWEIGHTED";

	if (direction == 0)
		weight = "DIRECTED";
	else
		weight = "UNDIRECTED";
	//     
	cout << "dumping graph: " << type << "   " << weight << "    vertices:" << vertexCount() << "    edges:" << edgeCount() << endl;
	cout << left << setw(25) << "VERTEX " << setw(50) << "ADJACENT VERTICES" << endl;
	cout << left << setw(25) << "-----------------" << setw(50) << "---------------------------------------------------" << endl;


	for (int i = 0; i <count; i++)
	{
		cout << setw(1) << "[" << setw(1) << i << setw(3) << "]" << setw(17) << node[i].info;
		for (int j = 0; j< node[i].countAdj; j++)
		{
			cout << "[" << node[i].edge[j].connIndex << "]" << node[node[i].edge[j].connIndex].info << "(" << node[i].edge[j].edgeWeight << ")    ";
		}
		cout << endl;
	}
}


bool Graph::isAdjacentTo(const Type& fromVertex, const Type& toVertex) const throw (std::logic_error)
{
	int indexFrom;
	int indexTo;

	bool isAdjacent = false;

	bool from_exists = false;
	bool to_exists = false;

	for (int i = 0; i< count; i++)
	{
		if (from_exists == false)
		{
			if (node[i].info == fromVertex)
			{
				from_exists = true;
				indexFrom = i;
			}
		}
		if (to_exists == false)
		{
			if (node[i].info == toVertex)
			{
				to_exists = true;
				indexTo = i;
			}
		}
	}

	try
	{
		if ((from_exists == false) || to_exists == false)
			throw std::logic_error("Either or both of the vertices don't exist in the graph. Cannot check adjacency");

		for (int i = 0; i< node[indexFrom].countAdj; i++)
		{
			if (indexTo == node[indexFrom].edge[i].connIndex)
			{
				isAdjacent = true;
				break;
			}
		}
	}
	catch (const std::logic_error bad_item)
	{
		cerr << "logic_error: " << bad_item.what() << '\n';
	}
	return isAdjacent;
}


int Graph::vertexCount() const
{
	return  count;
}


int Graph::edgeCount() const
{
	return  edgeCountNum;
}


int Graph::edgeWeight(const Type& fromVertex, const Type& toVertex) const throw (std::logic_error)
{
	int indexFrom;
	int indexTo;

	bool isAdjacent = false;

	bool from_exists = false;
	bool to_exists = false;

	bool edgeExist = false;

	int edgeWeightNum = -1;

	for (int i = 0; i< count; i++)
	{
		if (from_exists == false)
		{
			if (node[i].info == fromVertex)
			{
				from_exists = true;
				indexFrom = i;
			}
		}
		if (to_exists == false)
		{
			if (node[i].info == toVertex)
			{
				to_exists = true;
				indexTo = i;
			}
		}
	}
	try
	{
		for (int i = 0; i< node[indexFrom].countAdj; i++)
		{
			if (indexTo == node[indexFrom].edge[i].connIndex)
			{
				edgeExist = true;
				edgeWeightNum = node[indexFrom].edge[i].edgeWeight;
				break;
			}
		}
		if (edgeExist == false)
			throw std::logic_error("Following edege doesn't exist. -1");
	}
	catch (const std::logic_error bad_item)
	{
		cerr << "logic_error: " << bad_item.what() << '\n';
	}
	return edgeWeightNum;
}


void Graph::deleteEdge(const Type& fromVertex, const Type& toVertex) throw (std::logic_error)
{
	int indexFrom;
	int indexTo;

	bool isAdjacent = false;

	bool from_exists = false;
	bool to_exists = false;

	int edgeWeightNum;

	int deleteIndex;

	for (int i = 0; i< count; i++)
	{
		if (from_exists == false)
		{
			if (node[i].info == fromVertex)
			{
				from_exists = true;
				indexFrom = i;
			}
		}
		if (to_exists == false)
		{
			if (node[i].info == toVertex)
			{
				to_exists = true;
				indexTo = i;
			}
		}
	}
	if (direction == 0)
	{
		try
		{
			if ((from_exists == false) || to_exists == false)
				throw std::logic_error("Either or both of the vertices don't exist in the graph. Couldn't perform deletion");

			bool edgeExists = false;
			for (int i = 0; i < node[indexFrom].countAdj; i++)
			{
				if (node[indexFrom].edge[i].connIndex == indexTo)
				{
					deleteIndex = i;
					edgeExists = true;
					break;
				}
			}

			try
			{
				if (edgeExists == false)
					throw std::logic_error("Edge don't exist between the 2 vertices. Couldn't perform deletion");

				int j = 0;
				while (j != deleteIndex)
				{
					j++;
				}

				while (j < node[indexFrom].countAdj)
				{
					node[indexFrom].edge[j] = node[indexFrom].edge[j + 1];
					j++;
				}
				node[indexFrom].countAdj--;
			}
			catch (const std::logic_error bad_item)
			{
				cerr << "logic_error: " << bad_item.what() << '\n';
			}
		}
		catch (const std::logic_error bad_item)
		{
			cerr << "logic_error: " << bad_item.what() << '\n';
		}
	}
	else if (direction == 1) // it undirected we gotta delete from both
	{
		try
		{
			if ((from_exists == false) || to_exists == false)
				throw std::logic_error("Either or both of the vertices don't exist in the graph. Couldn't perform deletion");

			bool edgeExists = false;
			for (int i = 0; i < node[indexTo].countAdj; i++)
			{
				if (node[indexTo].edge[i].connIndex == indexFrom)
				{
					deleteIndex = i;
					edgeExists = true;
					break;
				}
			}

			int j = 0;
			while (j != deleteIndex)
			{
				j++;
			}

			while (j < node[indexTo].countAdj)
			{
				node[indexTo].edge[j] = node[indexTo].edge[j + 1];
				j++;
			}
			node[indexTo].countAdj--;

			bool edgeExistss = false;
			for (int i = 0; i < node[indexFrom].countAdj; i++)
			{
				if (node[indexFrom].edge[i].connIndex == indexTo)
				{
					deleteIndex = i;
					edgeExistss = true;
					break;
				}
			}
			int k = 0;
			while (k != deleteIndex)
			{
				k++;
			}

			while (k < node[indexFrom].countAdj)
			{
				node[indexFrom].edge[k] = node[indexFrom].edge[k + 1];
				k++;
			}
			node[indexFrom].countAdj--;
		}
		catch (const std::logic_error bad_item)
		{
			cerr << "logic_error: " << bad_item.what() << '\n';
		}
	}
	edgeCountNum -= 1;
}


void Graph::deleteVertex(const Type& vertex) throw (std::logic_error)
{
	int vertexIndexNumDelete;
	bool vertex_exists = false;

	int edgeWeightNum;

	int deleteIndex;

	for (int i = 0; i< count; i++)
	{
		if (vertex_exists == false)
		{
			if (node[i].info == vertex)
			{
				vertex_exists = true;
				vertexIndexNumDelete = i;
			}
		}
	}
	try
	{
		if (vertex_exists == false)
			throw std::logic_error("Vertex doesn't exist in the graph. Couldn't perform deletion");

		//first delete all the relationships that the vertex has with other vertices
		for (int i = 0; i<count; i++)
		{
			for (int j = 0; j < node[i].countAdj; j++)
			{
				if (node[i].edge[j].connIndex == vertexIndexNumDelete)
				{
					deleteEdge(node[i].info, node[vertexIndexNumDelete].info);
				}
				else {} //do nothing
			}
		}

		//then delete the vertex itself
		int j = 0;
		while (j != vertexIndexNumDelete)
		{
			j++;
		}

		while (j < count)
		{
			node[j] = node[j + 1];
			node[j].vertexIndex = j;
			j++;
		}
		count--;
	}
	catch (const std::logic_error bad_item)
	{
		cerr << "logic_error: " << bad_item.what() << '\n';
	}
}


int Graph::findVertex(const Type& vertex) const
{
	int vertexIndex = -1;
	bool exists = false;

	for (int i = 0; i< count; i++)
	{
		if (exists == false)
		{
			if (node[i].info == vertex)
			{
				exists = true;
				vertexIndex = i;
			}
		}
	}
	return vertexIndex;
}


void Graph::destroy()
{
	delete[] node;
	node = new Vertex[100];
	count = 0;
	edgeCountNum = 0;
}


Graph::~Graph()
{
	delete[] node;
}



const Graph& Graph::operator= (const Graph& otherGraph)
{
	node = new Vertex[100];

	weigh = otherGraph.weigh;
	direction = otherGraph.direction;
	count = otherGraph.count;
	edgeCountNum = otherGraph.edgeCountNum;

	for (int i = 0; i<otherGraph.count; i++)
	{
		node[i] = otherGraph.node[i];
	}
	return *this;
}

//copy constructor

Graph::Graph(const Graph& otherGraph)
{
	node = new Vertex[100];

	weigh = otherGraph.weigh;
	direction = otherGraph.direction;
	count = otherGraph.count;
	edgeCountNum = otherGraph.edgeCountNum;

	for (int i = 0; i<otherGraph.count; i++)
	{
		node[i] = otherGraph.node[i];
	}
}

int Graph::findRoad(const Type& vertex_dest, glm::vec3 origin) 
{
	int pathcount(1);
	Type startVertex;
	float min_dist = 1000;

	path = new glm::vec3[100];
	path[0] = origin;

	for (int i = 0; i < count; i++) {
		float temp = sqrt(pow((origin.x - node[i].info.pos.x), 2) + pow((origin.z - node[i].info.pos.z), 2));
		if (temp < min_dist) {
			startVertex = node[i].info;
			min_dist = temp;
		}
	}
	path[1] = startVertex.pos;
	pathcount++;

	if (startVertex == vertex_dest)
		return pathcount;

	
	if (isAdjacentTo(startVertex, vertex_dest))
	{
		path[2] = vertex_dest.pos;
		pathcount++;
		return pathcount;
	}
	
	if (startVertex == node[0].info) {
		path[2] = node[2].info.pos;
		path[3] = node[5].info.pos;
		path[4] = node[6].info.pos;
		path[5] = vertex_dest.pos;
		pathcount = 6;
	}
	else if (startVertex == node[1].info) {
		path[2] = node[3].info.pos;
		path[3] = node[4].info.pos;
		path[4] = node[7].info.pos;
		path[5] = node[6].info.pos;
		path[6] = vertex_dest.pos;
		pathcount = 7;
	}
	else if (startVertex == node[2].info || startVertex == node[5].info) {
		path[2] = node[5].info.pos;
		path[3] = node[6].info.pos;
		path[4] = vertex_dest.pos;
		pathcount = 5;
	}
	else if (startVertex == node[3].info || startVertex == node[4].info) {
		path[2] = node[4].info.pos;
		path[3] = node[7].info.pos;
		path[4] = node[6].info.pos;
		path[5] = vertex_dest.pos;
		pathcount = 6;
	}
	else if (startVertex == node[8].info || startVertex == node[9].info || startVertex == node[14].info || startVertex == node[12].info) {
		path[2] = node[8].info.pos;
		path[3] = node[11].info.pos;
		path[4] = vertex_dest.pos;
		pathcount = 5;
	}
	else if (startVertex == node[7].info || startVertex == node[13].info ) {
		path[2] = node[6].info.pos;
		path[3] = vertex_dest.pos;
		pathcount = 4;
	}
	return pathcount;
}
