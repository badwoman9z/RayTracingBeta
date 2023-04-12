#include"RenderManager.h"


void RenderManager::regisiterRenderPass(std::shared_ptr<RenderPass> pass) {
	renderPipeLine.push_back(pass);
}
void RenderManager::regisiterTexture(std::string s, unsigned int textureId) {
    textureMap[s] = textureId;
}
unsigned int RenderManager::getTexture(std::string s) {
    return textureMap[s];
}

void RenderManager::loadModel(std::string filepath, float materialID, float meshID, glm::mat4 trans, bool smoothNormal)
{
    // 顶点位置，索引
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> vertexTexcoord;
    std::vector<glm::vec3> vectexNormal;

    // 面片索引信息
    std::vector<int> positionIndex;
    std::vector<int> texcoordIndex;
    std::vector<int> normalIndex;

    // 打开文件流
    std::ifstream fin(filepath);
    std::string line;
    if (!fin.is_open()) {
        std::cout << "文件 " << filepath << " 打开失败" << std::endl;
        exit(-1);
    }

    bool hasNormal = true;
    // 按行读取
    while (std::getline(fin, line)) {
        std::istringstream sin(line);   // 以一行的数据作为 string stream 解析并且读取
        std::string type;
        GLfloat x, y, z;
        int v0, v1, v2;
        int vn0, vn1, vn2;
        int vt0, vt1, vt2;
        char slash;

        // 统计斜杆数目，用不同格式读取
        int slashCnt = 0;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == '/') slashCnt++;
        }
        // 读取obj文件
        sin >> type;
        if (type == "v") {
            sin >> x >> y >> z;


            vertices.push_back(glm::vec3(x, y, z));

        }
        if (type == "vt") {
            sin >> x >> y;
            vertexTexcoord.push_back(glm::vec2(x, y));
        }
        if (type == "vn") {
            sin >> x >> y >> z;



            vectexNormal.push_back(glm::vec3(x, y, z));
        }
        if (type == "f") {
            if (slashCnt == 6) {
                sin >> v0 >> slash >> vt0 >> slash >> vn0;
                sin >> v1 >> slash >> vt1 >> slash >> vn1;
                sin >> v2 >> slash >> vt2 >> slash >> vn2;
                positionIndex.push_back(v0 - 1);
                positionIndex.push_back(v1 - 1);
                positionIndex.push_back(v2 - 1);
                texcoordIndex.push_back(vt0 - 1);
                texcoordIndex.push_back(vt1 - 1);
                texcoordIndex.push_back(vt2 - 1);
                normalIndex.push_back(vn0 - 1);
                normalIndex.push_back(vn1 - 1);
                normalIndex.push_back(vn2 - 1);
                
            }
            else if (slashCnt == 0) {
                hasNormal = false;
                sin >> v0 >> v1 >> v2;
                positionIndex.emplace_back(v0 - 1);
                positionIndex.emplace_back(v1 - 1);
                positionIndex.emplace_back(v2 - 1);
                
            }
        }

        //}

        
    }
    
        for (auto& v : vertices) {
            glm::vec4 vv = glm::vec4(v.x, v.y, v.z, 1);
            vv = trans * vv;
            v = glm::vec3(vv.x, vv.y, vv.z);
        }
    

        if (hasNormal) {
            for (auto& n : vectexNormal) {
                 n = glm::transpose(glm::inverse(glm::mat3(trans))) * n;
             }
            std::cout << "has normal" << std::endl;
        }
        else {
            vectexNormal.resize(positionIndex.size(),glm::vec3(0));
            for (int i = 0; i < positionIndex.size(); i += 3) {
                glm::vec3 p1 = vertices[positionIndex[i]];
                glm::vec3 p2 = vertices[positionIndex[i + 1]];
                glm::vec3 p3 = vertices[positionIndex[i + 2]];
                glm::vec3 n = normalize(cross(p2 - p1, p3 - p1));
                vectexNormal[positionIndex[i]] += n;
                vectexNormal[positionIndex[i + 1]] += n;
                vectexNormal[positionIndex[i + 2]] += n;
            }
            std::cout << "build normal" << std::endl;
 
        }
    
 
  

    



    //构建 Triangle 对象数组
    int offset = triangles.size();  // 增量更新
    triangles.resize(offset + positionIndex.size() / 3);
    //

    for (int i = 0; i < positionIndex.size(); i += 3) {


        Triangle& t = triangles[offset + i / 3];
        // 传顶点属性
        t.p1 = vertices[positionIndex[i]];
        t.p2 = vertices[positionIndex[i + 1]];
        t.p3 = vertices[positionIndex[i + 2]];

        if (hasNormal) {
            t.n1 = vectexNormal[normalIndex[i]];
            t.n2 = vectexNormal[normalIndex[i + 1]];
            t.n3 = vectexNormal[normalIndex[i + 2]];
        }
        else {



            if (!smoothNormal) {
                glm::vec3 n = normalize(cross(t.p2 - t.p1, t.p3 - t.p1));
                t.n1 = n; t.n2 = n; t.n3 = n;
            }
            else {
                t.n1 = vectexNormal[positionIndex[i]];
                t.n2 = vectexNormal[positionIndex[i + 1]];
                t.n3 = vectexNormal[positionIndex[i + 2]];
            }

        }

        
        

         //传材质
        t.materialID = materialID;
        t.triangleID = offset + i / 3;

    }
    std::cout<<"load model finished" << std::endl;
    
    std::cout << triangles.size() << std::endl;

}


int RenderManager::BuildBVHTree(int l, int r, int n) {
    if (l > r) return 0;

    // 注：
    // 此处不可通过指针，引用等方式操作，必须用 nodes[id] 来操作
    // 因为 std::vector<> 扩容时会拷贝到更大的内存，那么地址就改变了
    // 而指针，引用均指向原来的内存，所以会发生错误
    bvhnodes.push_back(BVHNode());
    int id = bvhnodes.size() - 1;   // 注意： 先保存索引
    bvhnodes[id].left = bvhnodes[id].right = bvhnodes[id].n = bvhnodes[id].index = -1;
    bvhnodes[id].AA = glm::vec3(1145141919, 1145141919, 1145141919);
    bvhnodes[id].BB = glm::vec3(-1145141919, -1145141919, -1145141919);

    // 计算 AABB
    for (int i = l; i <= r; i++) {
        // 最小点 AA
        float minx = std::min(triangles[i].p1.x, std::min(triangles[i].p2.x, triangles[i].p3.x));
        float miny = std::min(triangles[i].p1.y, std::min(triangles[i].p2.y, triangles[i].p3.y));
        float minz = std::min(triangles[i].p1.z, std::min(triangles[i].p2.z, triangles[i].p3.z));
        bvhnodes[id].AA.x = std::min(bvhnodes[id].AA.x, minx);
        bvhnodes[id].AA.y = std::min(bvhnodes[id].AA.y, miny);
        bvhnodes[id].AA.z = std::min(bvhnodes[id].AA.z, minz);
        // 最大点 BB
        float maxx = std::max(triangles[i].p1.x, std::max(triangles[i].p2.x, triangles[i].p3.x));
        float maxy = std::max(triangles[i].p1.y, std::max(triangles[i].p2.y, triangles[i].p3.y));
        float maxz = std::max(triangles[i].p1.z, std::max(triangles[i].p2.z, triangles[i].p3.z));
        bvhnodes[id].BB.x = std::max(bvhnodes[id].BB.x, maxx);
        bvhnodes[id].BB.y = std::max(bvhnodes[id].BB.y, maxy);
        bvhnodes[id].BB.z = std::max(bvhnodes[id].BB.z, maxz);
    }

    // 不多于 n 个三角形 返回叶子节点
    if ((r - l + 1) <= n) {
        bvhnodes[id].n = r - l + 1;
        bvhnodes[id].index = l;
        return id;
    }

    // 否则递归建树
    float lenx = bvhnodes[id].BB.x - bvhnodes[id].AA.x;
    float leny = bvhnodes[id].BB.y - bvhnodes[id].AA.y;
    float lenz = bvhnodes[id].BB.z - bvhnodes[id].AA.z;
    // 按 x 划分
    if (lenx >= leny && lenx >= lenz)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpx);
    // 按 y 划分
    if (leny >= lenx && leny >= lenz)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpy);
    // 按 z 划分
    if (lenz >= lenx && lenz >= leny)
        std::sort(triangles.begin() + l, triangles.begin() + r + 1, cmpz);
    // 递归
    int mid = (l + r) / 2;
    int left = BuildBVHTree(l, mid, n);
    int right = BuildBVHTree(mid + 1, r, n);

    bvhnodes[id].left = left;
    bvhnodes[id].right = right;

    return id;


    std::cout << "build bvh finished" << std::endl;
}

void RenderManager::EncodeSSBOTriangle() {
    int n = triangles.size();
    triangles_encoded.resize(n);
    for (int i = 0; i < n; i++) {

        triangles_encoded[i].p1withTriangleID = glm::vec4(triangles[i].p1, triangles[i].triangleID);
        triangles_encoded[i].p2withTriangleID = glm::vec4(triangles[i].p2, triangles[i].triangleID);
        triangles_encoded[i].p3withTriangleID = glm::vec4(triangles[i].p3, triangles[i].triangleID);
        //法线
        triangles_encoded[i].n1withMaterialID = glm::vec4(triangles[i].n1, triangles[i].materialID);
        triangles_encoded[i].n2withMaterialID = glm::vec4(triangles[i].n2, triangles[i].materialID);
        triangles_encoded[i].n3withMaterialID = glm::vec4(triangles[i].n2, triangles[i].materialID);

    }
    std::cout << "encode triangle finished" << std::endl;

}
void RenderManager::EncodeSSBOBVH() {
    int n = bvhnodes.size();
    bvhnodes_encoded.resize(n);
    for (int i = 0; i < n; i++) {
        bvhnodes_encoded[i].left = bvhnodes[i].left;
        bvhnodes_encoded[i].right = bvhnodes[i].right;
        bvhnodes_encoded[i].index = bvhnodes[i].index;
        bvhnodes_encoded[i].n = bvhnodes[i].n;
        bvhnodes_encoded[i].AA = glm::vec4(bvhnodes[i].AA, 0.0);
        bvhnodes_encoded[i].BB = glm::vec4(bvhnodes[i].BB, 0.0);


    }
    std::cout << "encode bvh finished" << std::endl;
}


void RenderManager::init() {
    frameCount = std::make_shared<float>(0.0);
    projection = glm::perspective(glm::radians(RenderManager::get_instance().camera->Zoom), (float)RenderManager::get_instance().getWidth() / (float)RenderManager::get_instance().getHeight(), 0.1f, 30.0f);

    BuildBVHTree(0, triangles.size() - 1, 8);

    std::cout << bvhnodes.size() << std::endl;

    EncodeSSBOTriangle();
    EncodeSSBOBVH(); 

    Material m1;
    m1.baseColor = glm::vec3(0.0,0.2,0.3);
    m1.emessiveColor = glm::vec3(0.0);
    m1.metallic = 1.0;
    m1.roughness = 0.5;

    
    
    Material m2;
    m2.baseColor = glm::vec3(1.0);
    m2.emessiveColor = glm::vec3(0.0);
    m2.metallic = 1.0;
    m2.roughness = 0.03;
    Material m3;
    m3.baseColor = glm::vec3(1.0);
    m3.emessiveColor = glm::vec3(0);
    m3.metallic = 0;
    m3.roughness = 0.05;
    
    
    materials.push_back(m1);
    materials.push_back(m2);
    materials.push_back(m3);
    
    
    
    
    ssbo_triangles = std::make_shared<Buffer>(GL_SHADER_STORAGE_BUFFER, sizeof(SSBO_Triangle) * triangles_encoded.size(), &triangles_encoded[0], GL_STATIC_DRAW, 2);

    ssbo_bvhnodes = std::make_shared<Buffer>(GL_SHADER_STORAGE_BUFFER, sizeof(SSBO_BVHNode) * bvhnodes_encoded.size(), &bvhnodes_encoded[0], GL_STATIC_DRAW, 3);


    ssbo_material = std::make_shared<Buffer>(GL_SHADER_STORAGE_BUFFER, sizeof(Material) * materials.size(), &materials[0], GL_STATIC_DRAW, 4);
    
    //
    //std::cout << ssbo_triangles->getBufferID() << std::endl;
    //std::cout << ssbo_bvhnodes->getBufferID() << std::endl;


    for (auto pass : renderPipeLine) {
		pass->init();
	}
}

void RenderManager::update() {

    //if (lastViewMat != camera->GetViewMatrix()) {
    //    std::cout << "false" << std::endl;
    //}
    (*frameCount)++;

	for (auto pass : renderPipeLine) {
		//std::cout << pass->passName << std::endl;
		pass->update();
	}

    lastViewMat = camera->GetViewMatrix();
}


GLint RenderManager::getOrCreateScreenQuadVAO() {
    if (ScreenQuadVAO == 0)      //Fixed Me: some method to simply these code
        ScreenQuadVAO = createVAO4ScreenQuad();
    return ScreenQuadVAO;
}


