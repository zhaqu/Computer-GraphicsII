#include "ofApp.h"

float point[200000][3];
float point_color[200000];
int DFS_array[4000] = { -1 };
bool DFS_color[4000] = { 0 };
int faces[50000][4];
int point_num = 0;
int bound_num = 0;
int faces_num = 0;
float min_angle = 50.0;
float max_angle = -50.0;
CameraOrbit cam;
Mesh objMeshLoader;

void Mesh::ObjScan(const char* name)
{
    std::ifstream file;
    file.open(name);   
    char buf[256], str[255], *p;
    while(!file.getline(buf, 255).eof())
    {
        p = strtok(buf, " ");
        if(buf[0] == 'v')
        {
			if (buf[1] == NULL)
			{
				p = strtok(NULL, " ");
				point[point_num][0] = std::strtod(p, NULL);
				p = strtok(NULL, " ");
				point[point_num][1] = std::strtod(p, NULL);
				p = strtok(NULL, " ");
				point[point_num][2] = std::strtod(p, NULL);
				point_num += 1;
			}
        }
        else if (buf[0] == 'f')
        {
            p = strtok(NULL, " ");
            faces[faces_num][0] = std::atoi(p);
            p = strtok(NULL, " ");
            faces[faces_num][1] = std::atoi(p);
            p = strtok(NULL, " ");
            faces[faces_num][2] = std::atoi(p);
            if(p = strtok(NULL, " "))
            {
                faces[faces_num][3] = std::atoi(p);
            }
            else
            {
                faces[faces_num][3] = 0;
            }
            
            
            faces_num += 1;
        }
    }
}

void Mesh::InsertVertex(float x, float y, float z, int index)
{
    objMeshLoader.vertex[index]->pos = {x, y, z};
	objMeshLoader.vertex[index]->index = index;
}

void Mesh::InsertFace()
{
    for (size_t i = 0; i < faces_num; i++)
    {
        if(faces[i][3]>0)
        {
            for (size_t j = 0; j < 4; j++)
            {
            {
            objMeshLoader.half_edge[4*i+j]->vert = objMeshLoader.vertex[faces[i][j]-1];
			objMeshLoader.half_edge[4*i+j]->index = 4*i+j;
			objMeshLoader.vertex[faces[i][j]-1]->edge = objMeshLoader.half_edge[4*i+j];

			objMeshLoader.face[i]->index = i;
			objMeshLoader.face[i]->edge = objMeshLoader.half_edge[4*i];
			objMeshLoader.half_edge[4*i+j]->face = objMeshLoader.face[i];

            int next = (j+1) % 4;
			objMeshLoader.half_edge[4*i+j]->next = objMeshLoader.half_edge[4*i+next];
            }
            }
        }
        else
        {
            for (size_t j = 0; j < 3; j++)
            {
            {
			objMeshLoader.half_edge[4*i+j]->vert = objMeshLoader.vertex[faces[i][j]-1];
			objMeshLoader.half_edge[4*i+j]->index = 4*i+j;
			objMeshLoader.vertex[faces[i][j]-1]->edge = objMeshLoader.half_edge[4*i+j];

			objMeshLoader.face[i]->index = i;
			objMeshLoader.face[i]->edge = objMeshLoader.half_edge[4*i];
			objMeshLoader.half_edge[4*i+j]->face = objMeshLoader.face[i];

            int next = (j+1) % 3;
			objMeshLoader.half_edge[4*i+j]->next = objMeshLoader.half_edge[4*i+next];
            }
            }
			objMeshLoader.half_edge[4*i+3]->vert = NULL;
			objMeshLoader.half_edge[4*i+3]->index = -1;
			objMeshLoader.half_edge[4*i+3]->face = objMeshLoader.face[i];
        }
        
        
    }

}

std::vector<HalfEdge* > Vertex::vertex_Edges()
{
    std::vector<HalfEdge*> temp_edges;
    HalfEdge* start_vertex = this->edge;
    HalfEdge* temp_vertex = start_vertex;
    do
    {
        if(temp_vertex->opposite)
        {
            temp_vertex = temp_vertex->opposite;
        }
        else{break;}
        temp_vertex = temp_vertex->next;
        temp_edges.push_back(temp_vertex);
    } while (temp_vertex != start_vertex);
    return temp_edges;
}



std::vector<HalfEdge*> Face::face_Edges()
{
    std::vector<HalfEdge*> temp_edges;
    HalfEdge* start_face = this->edge;
    HalfEdge* temp_face = start_face;
    do
    {
        temp_face = temp_face->next;
        temp_edges.push_back(temp_face);
    } while (start_face != temp_face);
    return temp_edges;
}


void Mesh::BuildFromObj()
{
    int ecount = 4*faces_num;
	objMeshLoader.face.reserve(faces_num);
	objMeshLoader.vertex.reserve(point_num);
	objMeshLoader.half_edge.reserve(ecount);
	objMeshLoader.boundary_edge.reserve(ecount);

    for (size_t i = 0; i < point_num; i++)
    {
        Vertex* temp_b = new Vertex;
        temp_b->pos = {-1, -1, -1};
        temp_b->edge = NULL;
        temp_b->index = -1;
		objMeshLoader.vertex.push_back(temp_b);
    }
    for (size_t i = 0; i < faces_num; i++)
    {
        Face* temp = new Face;
        temp->edge = NULL;
        temp->index = -1;
		objMeshLoader.face.push_back(temp);
    }
    for (size_t i = 0; i < ecount; i++)
    {
        HalfEdge* temp = new HalfEdge;
        temp->face = NULL;
        temp->next = NULL;
        temp->opposite = NULL;
        temp->vert = NULL;
        temp->index = -1;
		objMeshLoader.half_edge.push_back(temp);
    }
    for (size_t i = 0; i < point_num; i++)
    {
        InsertVertex(point[i][0], point[i][1],point[i][2], i);
    }
    InsertFace();
    for (size_t i = 0; i < ecount; i++)
    {
        for (size_t j = i+1; j < ecount; j++)
        {
            Vertex* abegin = objMeshLoader.half_edge[i]->vert;
            if(!abegin) continue;
            Vertex* aend = objMeshLoader.half_edge[i]->next->vert;
            Vertex* bbegin = objMeshLoader.half_edge[j]->vert;
            if(!bbegin)continue;
            Vertex* bend = objMeshLoader.half_edge[j]->next->vert;
            if(abegin->index == bend->index && aend->index == bbegin->index)
            {
				objMeshLoader.half_edge[i]->opposite = objMeshLoader.half_edge[j];
				objMeshLoader.half_edge[i]->arrived = objMeshLoader.half_edge[j]->vert;
				objMeshLoader.half_edge[j]->opposite = objMeshLoader.half_edge[i];
				objMeshLoader.half_edge[j]->arrived = objMeshLoader.half_edge[i]->vert;
                break;
            }
        }
    }
    int boun_index = 0;
    for (size_t i = 0; i < ecount; i++)
    {
        if(!objMeshLoader.half_edge[i]->opposite && objMeshLoader.half_edge[i]->index>-1)
        {
            HalfEdge* boundary = new HalfEdge;
            boundary->face = NULL;
            boundary->index = boun_index;
            boun_index += 1;
            boundary->vert = objMeshLoader.half_edge[i]->next->vert;
            boundary->arrived = objMeshLoader.half_edge[i]->vert;
            boundary->opposite = objMeshLoader.half_edge[i];
			objMeshLoader.half_edge[i]->opposite = boundary;
            boundary->next = NULL;
            bound_num+=1;
			objMeshLoader.boundary_edge.push_back(boundary);
        }
    }
    for (size_t i = 0; i < bound_num; i++)
    {
        for (size_t j = 0; j < bound_num; j++)
        {
            if(objMeshLoader.boundary_edge[i]->arrived->index == objMeshLoader.boundary_edge[j]->vert->index)
            {
                objMeshLoader.boundary_edge[i]->next = objMeshLoader.boundary_edge[j];
            }
        }
    }
}

std::vector<std::vector<HalfEdge* > > Mesh::Boundaries()
{
	bool bon_include[5000] = { 0 };
	int vector_num = 0;
	std::vector<std::vector<HalfEdge* > > temp_edges;
	std::vector<HalfEdge* > single_bound;


	for (size_t i = 0; i < bound_num; i++)
	{

		if (bon_include[i]) { continue; };
		HalfEdge* start_edge = objMeshLoader.boundary_edge[i];
		HalfEdge* temp_edge = start_edge;
		do
		{
			temp_edge = temp_edge->next;
			single_bound.push_back(temp_edge);
			bon_include[temp_edge->index] = 1;
		} while (start_edge != temp_edge);
		vector_num += 1;
		temp_edges.push_back(single_bound);
		single_bound.clear();
	}
	return temp_edges;

}
float Vertex::vertex_color()
{
	std::vector<HalfEdge*> temp_edges;
	HalfEdge* start_vertex_edge = this->edge;
	HalfEdge* temp_vertex = start_vertex_edge;
	Vertex* start_vertex = start_vertex_edge->vert;
	for (size_t i = 0; i < bound_num; i++)
	{
		if (objMeshLoader.boundary_edge[i]->vert == start_vertex)
		{
			return -50.0;
		}
	}
	do
	{
		if (temp_vertex->opposite)
		{
			temp_vertex = temp_vertex->opposite;
		}
		else { break; }
		temp_vertex = temp_vertex->next;
		temp_edges.push_back(temp_vertex);
	} while (temp_vertex != start_vertex_edge);
	float angle = 2 * glm::pi<float>();
	for (size_t i = 0; i < temp_edges.size(); i++)
	{
		glm::vec3 edge_1 = glm::normalize(temp_edges[i]->arrived->pos - temp_edges[i]->vert->pos);
		glm::vec3 edge_2 = glm::normalize(temp_edges[(i + 1) % temp_edges.size()]->arrived->pos - temp_edges[(i + 1) % temp_edges.size()]->vert->pos);
		float temp_angle = glm::angle(edge_1, edge_2);
		angle -= temp_angle;
	}
	if (glm::abs(angle) < 0.00001)
	{
		angle = 0;
	}
	if (angle < min_angle)
	{
		min_angle = angle;
	}
	if (angle > max_angle)
	{
		max_angle = angle;
	}
	return angle;
}
float cotangent(float r)
{
	if (sin(r) == 0)
		return 999999;  //avoid crash

	return cos(r) / sin(r);
}
float Vertex::vertex_LA_color()
{
	std::vector<HalfEdge*> temp_edges;
	HalfEdge* start_vertex_edge = this->edge;
	HalfEdge* temp_vertex = start_vertex_edge;
	Vertex* start_vertex = start_vertex_edge->vert;
	for (size_t i = 0; i < bound_num; i++)
	{
		if (objMeshLoader.boundary_edge[i]->vert == start_vertex)
		{
			return -50.0;
		}
	}
	do
	{
		if (temp_vertex->opposite)
		{
			temp_vertex = temp_vertex->opposite;
		}
		else { break; }
		temp_vertex = temp_vertex->next;
		temp_edges.push_back(temp_vertex);
	} while (temp_vertex != start_vertex_edge);
	glm::vec3 start_pos = start_vertex->pos;
	glm::vec3 delta = glm::vec3(0.0);
#if !LACO
	for (size_t i = 0; i < temp_edges.size(); i++)
	{
		glm::vec3 pos_1 = temp_edges[i]->arrived->pos;
		delta += pos_1 ;
	}
	delta /= temp_edges.size();
	delta = start_pos - delta;
#else
	for (size_t i = 0; i < temp_edges.size(); i++)
	{
		glm::vec3 pos_1 = temp_edges[i]->next->vert->pos;
		glm::vec3 pos_2 = temp_edges[i]->next->arrived->pos;
		glm::vec3 pos_3 = temp_edges[i]->arrived->pos;
		glm::vec3 edge_1 = glm::normalize(pos_1 - pos_2);
		glm::vec3 edge_2 = glm::normalize(pos_3 - pos_2);
		float temp_angle = glm::angle(edge_1, edge_2);

		glm::vec3 pos_4 = temp_edges[i]->opposite->next->vert->pos;
		glm::vec3 pos_5 = temp_edges[i]->opposite->next->arrived->pos;
		glm::vec3 pos_6 = temp_edges[i]->arrived->pos;
		glm::vec3 edge_3 = glm::normalize(pos_4 - pos_5);
		glm::vec3 edge_4 = glm::normalize(pos_6 - pos_5);
		float temp_angle2 = glm::angle(edge_3, edge_4);
		float weight = 0.5*(cotangent(temp_angle) + cotangent(temp_angle2));
		delta += weight * (temp_edges[i]->arrived->pos - start_pos);
	}
#endif
	float re_color;
	re_color = glm::sqrt(glm::pow(delta.x, 2) + glm::pow(delta.y, 2) + glm::pow(delta.z, 2));
	if (glm::abs(re_color) < 0.00001)
	{
		re_color = 0;
	}
	//cout << delta.x << " " << delta.y << " " << delta.z << endl;
	if (re_color < min_angle)
	{
		min_angle = re_color;
	}
	if (re_color > max_angle)
	{
		max_angle = re_color;
	}
	return re_color;
}

void calculate_color()
{
	for (size_t i = 0; i < point_num; i++)
	{
#if AD
		point_color[i] = objMeshLoader.vertex[i]->vertex_color();
#elif LA
		point_color[i] = objMeshLoader.vertex[i]->vertex_LA_color();
#endif
	}
}

//--------------------------------------------------------------
void ofApp::setup() {
	//load the mesh and collect faces and vertices
#if BUNNY
	objMeshLoader.ObjScan("data/bunny.obj");
#endif
#if COW
	objMeshLoader.ObjScan("data/cow.obj");
#endif
#if LILIUM
	objMeshLoader.ObjScan("data/lilium.obj");
#endif
#if ROOF
	objMeshLoader.ObjScan("data/roof.obj");
#endif
#if DOUBLE_TORUS
	objMeshLoader.ObjScan("data/double_torus.obj");
#endif
#if MUSHROOM
	objMeshLoader.ObjScan("data/mushroom.obj");
#endif
#if DISK
	objMeshLoader.ObjScan("data/disk.obj");
#endif
#if AD
	objMeshLoader.BuildFromObj();
	calculate_color();
#elif LA
	objMeshLoader.BuildFromObj();
	calculate_color();
#endif
	int vertex_add_num = 0;
	for (int i = 0; i < faces_num; i++)
	{
		if (faces[i][3] > 0)
		{
			glm::vec3 point_0 = glm::vec3(point[faces[i][0] - 1][0], point[faces[i][0] - 1][1], point[faces[i][0] - 1][2]);
			glm::vec3 point_1 = glm::vec3(point[faces[i][1] - 1][0], point[faces[i][1] - 1][1], point[faces[i][1] - 1][2]);
			glm::vec3 point_2 = glm::vec3(point[faces[i][2] - 1][0], point[faces[i][2] - 1][1], point[faces[i][2] - 1][2]);
			glm::vec3 point_3 = glm::vec3(point[faces[i][3] - 1][0], point[faces[i][3] - 1][1], point[faces[i][3] - 1][2]);
			mesh.addVertex(point_0);
			mesh.addVertex(point_1);
			mesh.addVertex(point_2);
			mesh.addVertex(point_3);
			for (int j = 0; j < 4; j++)
			{
				ofMesh tmp_mesh;
#if BUNNY
				Cylinder(glm::vec3(point[faces[i][j] - 1][0], point[faces[i][j] - 1][1], point[faces[i][j] - 1][2]), glm::vec3(point[faces[i][(j + 1) % 4] - 1][0], point[faces[i][(j + 1) % 4] - 1][1], point[faces[i][(j + 1) % 4] - 1][2]), 0.0001, tmp_mesh);
#elif MUSHROOM
				Cylinder(glm::vec3(point[faces[i][j] - 1][0], point[faces[i][j] - 1][1], point[faces[i][j] - 1][2]), glm::vec3(point[faces[i][(j + 1) % 4] - 1][0], point[faces[i][(j + 1) % 4] - 1][1], point[faces[i][(j + 1) % 4] - 1][2]), 0.3, tmp_mesh);
#elif DOUBLE_TORUS
				Cylinder(glm::vec3(point[faces[i][j] - 1][0], point[faces[i][j] - 1][1], point[faces[i][j] - 1][2]), glm::vec3(point[faces[i][(j + 1) % 4] - 1][0], point[faces[i][(j + 1) % 4] - 1][1], point[faces[i][(j + 1) % 4] - 1][2]), 0.3, tmp_mesh);
#elif DISK
				Cylinder(glm::vec3(point[faces[i][j] - 1][0], point[faces[i][j] - 1][1], point[faces[i][j] - 1][2]), glm::vec3(point[faces[i][(j + 1) % 4] - 1][0], point[faces[i][(j + 1) % 4] - 1][1], point[faces[i][(j + 1) % 4] - 1][2]), 0.3, tmp_mesh);

#else
				Cylinder(glm::vec3(point[faces[i][j] - 1][0], point[faces[i][j] - 1][1], point[faces[i][j] - 1][2]), glm::vec3(point[faces[i][(j + 1) % 4] - 1][0], point[faces[i][(j + 1) % 4] - 1][1], point[faces[i][(j + 1) % 4] - 1][2]), 0.01, tmp_mesh);
#endif
				edge_mesh.push_back(tmp_mesh);
			}
			mesh.addTriangle(vertex_add_num, vertex_add_num + 1, vertex_add_num + 3);
			mesh.addTriangle(vertex_add_num + 1, vertex_add_num + 2, vertex_add_num + 3);
			glm::vec3 edge_1 = point_1 - point_0;
			glm::vec3 edge_2 = point_3 - point_0;
			glm::vec3 ve_normal = glm::normalize(glm::cross(edge_2, edge_1));
			mesh.addNormal(-ve_normal);
			mesh.addNormal(-ve_normal);
			mesh.addNormal(-ve_normal);
			mesh.addNormal(-ve_normal);
			vertex_add_num += 4;
		}
		else
		{
			glm::vec3 point_0 = glm::vec3(point[faces[i][0] - 1][0], point[faces[i][0] - 1][1], point[faces[i][0] - 1][2]);
			glm::vec3 point_1 = glm::vec3(point[faces[i][1] - 1][0], point[faces[i][1] - 1][1], point[faces[i][1] - 1][2]);
			glm::vec3 point_2 = glm::vec3(point[faces[i][2] - 1][0], point[faces[i][2] - 1][1], point[faces[i][2] - 1][2]);
			mesh.addVertex(point_0);
			mesh.addVertex(point_1);
			mesh.addVertex(point_2);
			for (int j = 0; j < 3; j++)
			{
				ofMesh tmp_mesh;
#if BUNNY
				Cylinder(glm::vec3(point[faces[i][j] - 1][0], point[faces[i][j] - 1][1], point[faces[i][j] - 1][2]), glm::vec3(point[faces[i][(j + 1) % 3] - 1][0], point[faces[i][(j + 1) % 3] - 1][1], point[faces[i][(j + 1) % 3] - 1][2]), 0.0001, tmp_mesh);
#elif	MUSHROOM
				Cylinder(glm::vec3(point[faces[i][j] - 1][0], point[faces[i][j] - 1][1], point[faces[i][j] - 1][2]), glm::vec3(point[faces[i][(j + 1) % 3] - 1][0], point[faces[i][(j + 1) % 3] - 1][1], point[faces[i][(j + 1) % 3] - 1][2]), 0.3, tmp_mesh);
#elif	DOUBLE_TORUS
				Cylinder(glm::vec3(point[faces[i][j] - 1][0], point[faces[i][j] - 1][1], point[faces[i][j] - 1][2]), glm::vec3(point[faces[i][(j + 1) % 3] - 1][0], point[faces[i][(j + 1) % 3] - 1][1], point[faces[i][(j + 1) % 3] - 1][2]), 0.3, tmp_mesh);
#elif	DISK
				Cylinder(glm::vec3(point[faces[i][j] - 1][0], point[faces[i][j] - 1][1], point[faces[i][j] - 1][2]), glm::vec3(point[faces[i][(j + 1) % 3] - 1][0], point[faces[i][(j + 1) % 3] - 1][1], point[faces[i][(j + 1) % 3] - 1][2]), 0.3, tmp_mesh);
#else
				Cylinder(glm::vec3(point[faces[i][j] - 1][0], point[faces[i][j] - 1][1], point[faces[i][j] - 1][2]), glm::vec3(point[faces[i][(j + 1) % 3] - 1][0], point[faces[i][(j + 1) % 3] - 1][1], point[faces[i][(j + 1) % 3] - 1][2]), 0.01, tmp_mesh);
#endif
				edge_mesh.push_back(tmp_mesh);
			}
			mesh.addTriangle(vertex_add_num, vertex_add_num + 1, vertex_add_num + 2);
			glm::vec3 edge_1 = point_1 - point_0;
			glm::vec3 edge_2 = point_2 - point_0;
			glm::vec3 ve_normal = glm::normalize(glm::cross(edge_2, edge_1));
			
			mesh.addNormal(-ve_normal);
			mesh.addNormal(-ve_normal);
			mesh.addNormal(-ve_normal);
#if AD
			float angle_length = max_angle - min_angle;
			float mean_angle = min_angle + angle_length / 2.0;
			float vertex_color[3];
			for (size_t j = 0; j < 3; j++)
			{
				vertex_color[j] = point_color[faces[i][j] - 1];
			}
			if (angle_length != 0)
			{
				for (size_t j = 0; j < 3; j++)
				{
					if (vertex_color[j] > -51 && vertex_color[j] < -49)
					{
						mesh.addColor(ofDefaultColorType(0.0, 0.0, 0.0));
					}
					else
					{
						float temp = (vertex_color[j]-min_angle) / angle_length;
						if (temp < 0.25)
						{
							mesh.addColor(ofDefaultColorType(0.0, temp * 4.0, 1.0));

						}
						else if (temp >=0.25 && temp < 0.5)
						{
							mesh.addColor(ofDefaultColorType(0.0, 1.0, 1.0- (temp-0.25)*4.0));
						}
						else if (temp>=0.5 && temp<0.75)
						{
							mesh.addColor(ofDefaultColorType((temp-0.5)*4, 1.0, 0.0));
						}
						else
						{
							mesh.addColor(ofDefaultColorType(1.0, 1.0-(temp-0.75)*4, 0.0));
						}
						
					}
				}
			}
			else
			{
				for (size_t j = 0; j < 3; j++)
				{
					if (vertex_color[j] > -51 && vertex_color[j] < -49)
					{
						mesh.addColor(ofDefaultColorType(0.0, 0.0, 0.0));
					}
					else
					{
						mesh.addColor(ofDefaultColorType(0.0, 0.0, 1.0));
					}
				}
			}
#elif LA
			float vertex_color[3];
			for (size_t j = 0; j < 3; j++)
			{
				vertex_color[j] = point_color[faces[i][j] - 1];
			}
			float total_length = max_angle - min_angle;
			if (total_length != 0)
			{
				for (size_t j = 0; j < 3; j++)
				{
					float temp_color = vertex_color[j];
					if (temp_color > -51 && temp_color < -49)
					{
						mesh.addColor(ofDefaultColorType(0.0, 0.0, 0.0));
					}
					else
					{
						float temp = (vertex_color[j] - min_angle) / total_length;
						if (temp < 0.25)
						{
							mesh.addColor(ofDefaultColorType(0.0, temp * 4.0, 1.0));

						}
						else if (temp >= 0.25 && temp < 0.5)
						{
							mesh.addColor(ofDefaultColorType(0.0, 1.0, 1.0 - (temp - 0.25)*4.0));
						}
						else if (temp >= 0.5 && temp < 0.75)
						{
							mesh.addColor(ofDefaultColorType((temp - 0.5) * 4, 1.0, 0.0));
						}
						else
						{
							mesh.addColor(ofDefaultColorType(1.0, 1.0 - (temp - 0.75) * 4, 0.0));
						}
						//cout << min_la.x << " " << min_la.y << " " << min_la.z << endl;
						//cout << temp << endl;
					}
				}
			}
			else
			{
				for (size_t j = 0; j < 3; j++)
				{
					float temp_color = vertex_color[j];
					if (temp_color > -51 && temp_color < -49)
					{
						mesh.addColor(ofDefaultColorType(0.0, 0.0, 0.0));
					}
					else
					{
						mesh.addColor(ofDefaultColorType(0.0, 0.0, 1.0));
					}
				}
			}
#endif
			vertex_add_num += 3;
		}

	}

	shader.load("vertex_color.vert", "vertex_color.frag");
	edge_shader.load("edge.vert", "edge.frag");
	//mesh.enableColors();
	//calculate the bound and center


	//calculate lookat matrix
	view = glm::lookAt(eye, center, up);
	//calculate projection matrix
	projection = glm::perspective(glm::radians(60.0f), (float)ofGetHeight() / (float)ofGetWidth(), 0.1f, 1000.0f);
	//light.setPosition(1, 0, -1);
	//light.setDirectional();
	//light.setDiffuseColor(ofColor(0.f, 0.f, 255.f));
	//light.setSpecularColor(ofColor(255.f, 255.f, 255.f));
	//cam.enableMouseInput();
	//cam.enableMouseMiddleButton();
	cam.Zoom(3);
	translate_matrix = glm::translate(-mesh.getCentroid());
	rotate_matrix = glm::mat4(1.0);
#if BUNNY
	glm::mat4 scale_matrix_BUNNY = glm::scale(glm::vec3(10.0));
#endif
	glm::mat4 scale_matrix = glm::scale(glm::vec3(0.1, 0.1, 0.1));
	model = scale_matrix * rotate_matrix * translate_matrix;
#if BUNNY
	model = scale_matrix_BUNNY * model;
#endif
	cam.RotateHorizotal(0.3);

}
//--------------------------------------------------------------
void ofApp::update(){
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofEnableDepthTest();
	//ofSetColor(100, 100, 100);
	//light.enable();
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 0.7f);
	//ofColor(255, 255, 100);
	//meshNode.transformGL();
	shader.begin();
	shader.setUniformMatrix4f("model", model);
	shader.setUniformMatrix4f("view", cam.ViewMatrix());
	shader.setUniformMatrix4f("projection", cam.ProjectionMatrix());
	shader.setUniform3f("cam_pos", cam.Pos());
	shader.setUniform3f("l_diffuse", l_diffuse);
	//shader.setUniform4f("l_ambient", l_ambient);
	shader.setUniform3f("l_specular", l_specular);
	shader.setUniform3f("m_diffuse", m_diffuse);
	shader.setUniform3f("m_ambient", m_ambient);
	shader.setUniform3f("m_specular", m_specular);
	shader.setUniform3f("m_emission", m_emission);
	shader.setUniform1f("shininess", shininess);
	/*shader.setUniformMatrix4f("projection", projection);
	shader.setUniformMatrix4f("modelview", vm);
	shader.setUniformMatrix4f("normalMat", normal_matrix);*/
	
	mesh.draw();
	
	//light.draw();
	shader.end();
	glDisable(GL_POLYGON_OFFSET_FILL);
	//meshNode.restoreTransformGL();
	edge_shader.begin();
	edge_shader.setUniformMatrix4f("model", model);
	edge_shader.setUniformMatrix4f("view", cam.ViewMatrix());
	edge_shader.setUniformMatrix4f("projection", cam.ProjectionMatrix());
	for (size_t i = 0; i < edge_mesh.size(); i++)
	{
		edge_mesh[i].draw();
	}
	edge_shader.end();
	//light.disable();

	ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	//left click with rotation
	if (button == 2)
	{
		int temp = x - old_x + old_y - y;
		if (temp < 0)
			cam.Zoom(-0.1);
		else if (temp > 0)
			cam.Zoom(0.1);
		else
			;
		old_x = x;
		old_y = y;
	}
	if (button == 0)
	{
		float x_degree = (x - old_x) * 0.001;
		float y_degree = (y - old_y) * 0.001;
		old_x = x;
		old_y = y;
		cam.RotateHorizotal(x_degree);
		cam.RotateVertical(y_degree);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	//left click with rotation
	old_x = x;
	old_y = y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	//left click with rotation
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
