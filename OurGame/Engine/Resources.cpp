#include "pch.h"
#include "Resources.h"
#include "Engine.h"

namespace hm
{
    Resources::Resources()
    {

    }

    Resources::~Resources()
    {

    }

    void Resources::Initialize()
    {
        CreateDefaultShader();
        CreateDefaultMaterial();
    }
    void Resources::Destroy()
    {
    }
    shared_ptr<Texture> Resources::CreateTexture(const wstring& _name, DXGI_FORMAT _eFormat, UINT32 _eTextureType, int _width, int _height, bool _bMultiSampling)
    {
        shared_ptr<Texture> pTexture = make_shared<Texture>();
        pTexture->Create(_eTextureType, _eFormat, _width, _height, _bMultiSampling);
        Add(_name, pTexture);
        pTexture->SetName(_name);

        return pTexture;
    }
    shared_ptr<Texture> Resources::CreateTextureFromResource(const wstring& _name, DXGI_FORMAT _eFormat, UINT32 _eTextureType, ComPtr<ID3D11Texture2D> _tex2D, bool _bMultiSampling)
    {
        shared_ptr<Texture> pTexture = make_shared<Texture>();
        pTexture->CreateFromTexture(_eTextureType, _eFormat, _tex2D, _bMultiSampling);
        Add(_name, pTexture);

        return pTexture;
    }
    shared_ptr<MeshData> Resources::LoadFBX(const wstring& _path, const wstring& _shaderName, bool _bInvNormal)
    {
        wstring key = _path;
        wstring ext = fs::path(_path).extension();
        shared_ptr<MeshData> pMeshData = nullptr;

        if (ext == L".fbx")
        {
            key = ChangeFileExt(_path, L"msh");

            pMeshData = Get<MeshData>(key);
            if (nullptr != pMeshData)
                return pMeshData;

            else
            {
                if (true == fs::exists(key))
                {
                    pMeshData = make_shared<MeshData>();
                    pMeshData->Load(key);
                }
                else
                {
                    pMeshData = MeshData::LoadFromFBX(_path, _shaderName, _bInvNormal);
                    pMeshData->SetName(key);
                }
            }
        }

        else if (ext == L".msh")
        {
            pMeshData = Get<MeshData>(key);
            if (nullptr != pMeshData)
                return pMeshData;

            else
            {
                pMeshData = make_shared<MeshData>();
                pMeshData->Load(_path);
            }
        }

        else
        {
            AssertEx(false, L"Resources::LoadFBX() - 잘못된 확장자");
        }
        
        Add(key, pMeshData);
        return pMeshData;
    }
    shared_ptr<Mesh> Resources::LoadRectMesh()
    {
        shared_ptr<Mesh> pFindMesh = Get<Mesh>(L"Rect");
        if (pFindMesh)
            return pFindMesh;

        std::vector<Vertex> vertices;

        vertices.push_back(Vertex(Vec3(-1.f, 1.f, 0.f), Vec2(0.f, 0.f), Vec3(0.f, 0.f, -1.f), Vec3(1.0f, 0.0f, 0.0f)));
        vertices.push_back(Vertex(Vec3(1.f, 1.f, 0.f), Vec2(1.f, 0.f), Vec3(0.f, 0.f, -1.f), Vec3(1.0f, 0.0f, 0.0f)));
        vertices.push_back(Vertex(Vec3(1.f, -1.f, 0.f), Vec2(1.f, 1.f), Vec3(0.f, 0.f, -1.f), Vec3(1.0f, 0.0f, 0.0f)));
        vertices.push_back(Vertex(Vec3(-1.f, -1.f, 0.f), Vec2(0.f, 1.f), Vec3(0.f, 0.f, -1.f), Vec3(1.0f, 0.0f, 0.0f)));

        std::vector<int> indices(6);

        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;

        indices[3] = 0;
        indices[4] = 2;
        indices[5] = 3;

        shared_ptr<Mesh> pMesh = make_shared<Mesh>();
        pMesh->Initialize(vertices, indices);

        Add<Mesh>(L"Rect", pMesh);

        return pMesh;
    }
    shared_ptr<Mesh> Resources::LoadCircleMesh()
    {
        return shared_ptr<Mesh>();
    }
    shared_ptr<Mesh> Resources::LoadPointMesh()
    {
        shared_ptr<Mesh> pFindMesh = Get<Mesh>(L"Point");
        if (pFindMesh)
            return pFindMesh;

        std::vector<Vertex> vertices(1);
        vertices[0] = Vertex(Vec3(0.f, 0.f, 0.f), Vec2(0.5f, 0.5f), Vec3(0.f, 0.f, -1.f), Vec3(1.0f, 0.0f, 0.0f));


        std::vector<int> indices(1);
        indices[0] = 0;

        shared_ptr<Mesh> pMesh = make_shared<Mesh>();
        pMesh->Initialize(vertices, indices);

        Add<Mesh>(L"Point", pMesh);
        return pMesh;
    }
    shared_ptr<Mesh> Resources::LoadCubeMesh()
    {
        shared_ptr<Mesh> pFindMesh = Get<Mesh>(L"Cube");
        if (pFindMesh)
            return pFindMesh;

        float w2 = 0.5f;
        float h2 = 0.5f;
        float d2 = 0.5f;

        std::vector<Vertex> vec(24);

        // 앞면
        vec[0] = Vertex(Vec3(-w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
        vec[1] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
        vec[2] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
        vec[3] = Vertex(Vec3(+w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f), Vec3(1.0f, 0.0f, 0.0f));
        // 뒷면
        vec[4] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
        vec[5] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
        vec[6] = Vertex(Vec3(+w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
        vec[7] = Vertex(Vec3(-w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f));
        // 윗면
        vec[8] = Vertex(Vec3(-w2, +h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
        vec[9] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
        vec[10] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
        vec[11] = Vertex(Vec3(+w2, +h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, 1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f));
        // 아랫면
        vec[12] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
        vec[13] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
        vec[14] = Vertex(Vec3(+w2, -h2, +d2), Vec2(0.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
        vec[15] = Vertex(Vec3(-w2, -h2, +d2), Vec2(1.0f, 0.0f), Vec3(0.0f, -1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f));
        // 왼쪽면
        vec[16] = Vertex(Vec3(-w2, -h2, +d2), Vec2(0.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
        vec[17] = Vertex(Vec3(-w2, +h2, +d2), Vec2(0.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
        vec[18] = Vertex(Vec3(-w2, +h2, -d2), Vec2(1.0f, 0.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
        vec[19] = Vertex(Vec3(-w2, -h2, -d2), Vec2(1.0f, 1.0f), Vec3(-1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
        // 오른쪽면
        vec[20] = Vertex(Vec3(+w2, -h2, -d2), Vec2(0.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
        vec[21] = Vertex(Vec3(+w2, +h2, -d2), Vec2(0.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
        vec[22] = Vertex(Vec3(+w2, +h2, +d2), Vec2(1.0f, 0.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));
        vec[23] = Vertex(Vec3(+w2, -h2, +d2), Vec2(1.0f, 1.0f), Vec3(1.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 1.0f));

        std::vector<int> idx(36);

        // 앞면
        idx[0] = 0; idx[1] = 1; idx[2] = 2;
        idx[3] = 0; idx[4] = 2; idx[5] = 3;
        // 뒷면
        idx[6] = 4; idx[7] = 5; idx[8] = 6;
        idx[9] = 4; idx[10] = 6; idx[11] = 7;
        // 윗면
        idx[12] = 8; idx[13] = 9; idx[14] = 10;
        idx[15] = 8; idx[16] = 10; idx[17] = 11;
        // 아랫면
        idx[18] = 12; idx[19] = 13; idx[20] = 14;
        idx[21] = 12; idx[22] = 14; idx[23] = 15;
        // 왼쪽면
        idx[24] = 16; idx[25] = 17; idx[26] = 18;
        idx[27] = 16; idx[28] = 18; idx[29] = 19;
        // 오른쪽면
        idx[30] = 20; idx[31] = 21; idx[32] = 22;
        idx[33] = 20; idx[34] = 22; idx[35] = 23;

        shared_ptr<Mesh> pMesh = make_shared<Mesh>();
        pMesh->Initialize(vec, idx);
        Add(L"Cube", pMesh);

        return pMesh;
    }
    shared_ptr<Mesh> Resources::LoadSphereMesh()
    {
        shared_ptr<Mesh> pFindMesh = Get<Mesh>(L"Sphere");
        if (pFindMesh)
            return pFindMesh;

        float radius = 0.5f; // 구의 반지름
        int stackCount = 20; // 가로 분할
        int sliceCount = 20; // 세로 분할

        std::vector<Vertex> vec;

        Vertex v;

        // 북극
        v.pos = Vec3(0.0f, radius, 0.0f);
        v.uv = Vec2(0.5f, 0.0f);
        v.normal = v.pos;
        v.normal.Normalize();
        v.tangent = Vec3(1.0f, 0.0f, 1.0f);
        vec.push_back(v);

        float stackAngle = XM_PI / stackCount;
        float sliceAngle = XM_2PI / sliceCount;

        float deltaU = 1.f / static_cast<float>(sliceCount);
        float deltaV = 1.f / static_cast<float>(stackCount);

        // 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
        for (int y = 1; y <= stackCount - 1; ++y)
        {
            float phi = y * stackAngle;

            // 고리에 위치한 정점
            for (int x = 0; x <= sliceCount; ++x)
            {
                float theta = x * sliceAngle;

                v.pos.x = radius * sinf(phi) * cosf(theta);
                v.pos.y = radius * cosf(phi);
                v.pos.z = radius * sinf(phi) * sinf(theta);

                v.uv = Vec2(deltaU * x, deltaV * y);

                v.normal = v.pos;
                v.normal.Normalize();

                v.tangent.x = -radius * sinf(phi) * sinf(theta);
                v.tangent.y = 0.0f;
                v.tangent.z = radius * sinf(phi) * cosf(theta);
                v.tangent.Normalize();

                vec.push_back(v);
            }
        }

        // 남극
        v.pos = Vec3(0.0f, -radius, 0.0f);
        v.uv = Vec2(0.5f, 1.0f);
        v.normal = v.pos;
        v.normal.Normalize();
        v.tangent = Vec3(1.0f, 0.0f, 0.0f);
        vec.push_back(v);

        std::vector<int> idx(36);

        // 북극 인덱스
        for (int i = 0; i <= sliceCount; ++i)
        {
            //  [0]
            //   |  \
    		//  [i+1]-[i+2]
            idx.push_back(0);
            idx.push_back(i + 2);
            idx.push_back(i + 1);
        }

        // 몸통 인덱스
        int ringVertexCount = sliceCount + 1;
        for (int y = 0; y < stackCount - 2; ++y)
        {
            for (int x = 0; x < sliceCount; ++x)
            {
                //  [y, x]-[y, x+1]
                //  |		/
                //  [y+1, x]
                idx.push_back(1 + (y)*ringVertexCount + (x));
                idx.push_back(1 + (y)*ringVertexCount + (x + 1));
                idx.push_back(1 + (y + 1) * ringVertexCount + (x));
                //		 [y, x+1]
                //		 /	  |
                //  [y+1, x]-[y+1, x+1]
                idx.push_back(1 + (y + 1) * ringVertexCount + (x));
                idx.push_back(1 + (y)*ringVertexCount + (x + 1));
                idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
            }
        }

        // 남극 인덱스
        int bottomIndex = static_cast<int>(vec.size()) - 1;
        int lastRingStartIndex = bottomIndex - ringVertexCount;
        for (int i = 0; i < sliceCount; ++i)
        {
            //  [last+i]-[last+i+1]
            //  |      /
            //  [bottom]
            idx.push_back(bottomIndex);
            idx.push_back(lastRingStartIndex + i);
            idx.push_back(lastRingStartIndex + i + 1);
        }

        shared_ptr<Mesh> pMesh = make_shared<Mesh>();
        pMesh->Initialize(vec, idx);
        Add(L"Sphere", pMesh);

        return pMesh;
    }
    shared_ptr<Mesh> Resources::LoadBoundingCubeMesh()
    {
        shared_ptr<Mesh> pFindMesh = Get<Mesh>(L"BoundingCube");
        if (pFindMesh)
            return pFindMesh;

        float w2 = 0.5f;
        float h2 = 0.5f;
        float d2 = 0.5f;

        std::vector<Vertex> vec(8);

        // 8개의 정점을 설정 (큐브의 모서리 점들)
        vec[0] = Vertex(Vec3(-w2, -h2, -d2));
        vec[1] = Vertex(Vec3(-w2, +h2, -d2));
        vec[2] = Vertex(Vec3(+w2, +h2, -d2));
        vec[3] = Vertex(Vec3(+w2, -h2, -d2));
        vec[4] = Vertex(Vec3(-w2, -h2, +d2));
        vec[5] = Vertex(Vec3(+w2, -h2, +d2));
        vec[6] = Vertex(Vec3(+w2, +h2, +d2));
        vec[7] = Vertex(Vec3(-w2, +h2, +d2));

        std::vector<int> idx(24);

        // 큐브의 모든 선분을 설정 (linelist를 위해 인덱스 설정)
        idx[0] = 0; idx[1] = 1;
        idx[2] = 1; idx[3] = 2;
        idx[4] = 2; idx[5] = 3;
        idx[6] = 3; idx[7] = 0;
        idx[8] = 4; idx[9] = 5;
        idx[10] = 5; idx[11] = 6;
        idx[12] = 6; idx[13] = 7;
        idx[14] = 7; idx[15] = 4;
        idx[16] = 0; idx[17] = 4;
        idx[18] = 1; idx[19] = 7;
        idx[20] = 2; idx[21] = 6;
        idx[22] = 3; idx[23] = 5;

        shared_ptr<Mesh> pMesh = make_shared<Mesh>();
        pMesh->Initialize(vec, idx);
        Add(L"BoundingCube", pMesh);

        return pMesh;
    }
    shared_ptr<Mesh> Resources::CreateCapsuleMesh(float _radius, float _height)
    {
        float radius = _radius; // 반구의 반지름
        float height = _height; // 원기둥의 높이
        int stackCount = 10; // 가로 분할
        int sliceCount = 10; // 세로 분할

        std::vector<Vertex> vec;

        Vertex v;

        // 상단 반구 생성
        // 북극
        v.pos = Vec3(height / 2 + radius, 0.0f, 0.0f);
        v.uv = Vec2(0.5f, 0.0f);
        v.normal = v.pos;
        v.normal.Normalize();
        v.tangent = Vec3(1.0f, 0.0f, 1.0f);
        vec.push_back(v);

        float stackAngle = XM_PI / stackCount;
        float sliceAngle = XM_2PI / sliceCount;

        float deltaU = 1.f / static_cast<float>(sliceCount);
        float deltaV = 1.f / static_cast<float>(stackCount);

        // 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
        for (int y = 1; y <= stackCount / 2; ++y)
        {
            float phi = y * stackAngle;

            // 고리에 위치한 정점
            for (int x = 0; x <= sliceCount; ++x)
            {
                float theta = x * sliceAngle;

                v.pos.x = height / 2 + radius * cosf(phi);
                v.pos.y = radius * sinf(phi) * cosf(theta);
                v.pos.z = radius * sinf(phi) * sinf(theta);

                v.uv = Vec2(deltaU * x, deltaV * y);

                v.normal = v.pos;
                v.normal.Normalize();

                v.tangent.x = -radius * sinf(phi) * sinf(theta);
                v.tangent.y = 0.0f;
                v.tangent.z = radius * sinf(phi) * cosf(theta);
                v.tangent.Normalize();

                vec.push_back(v);
            }
        }

        // 원기둥 생성
        for (int y = stackCount / 2 + 1; y <= stackCount - stackCount / 2 - 1; ++y)
        {
            float phi = y * stackAngle;

            // 고리에 위치한 정점
            for (int x = 0; x <= sliceCount; ++x)
            {
                float theta = x * sliceAngle;

                v.pos.x = height / 2 - height * (y - stackCount / 2) / (stackCount - stackCount / 2);
                v.pos.y = radius * cosf(theta);
                v.pos.z = radius * sinf(theta);

                v.normal.x = cosf(theta);
                v.normal.y = 0;
                v.normal.z = sinf(theta);

                v.tangent.x = -sinf(theta);
                v.tangent.y = 0.0f;
                v.tangent.z = cosf(theta);

                vec.push_back(v);
            }
        }

        // 하단 반구 생성
        for (int y = stackCount - stackCount / 2; y <= stackCount - 1; ++y)
        {
            float phi = y * stackAngle;

            // 고리에 위치한 정점
            for (int x = 0; x <= sliceCount; ++x)
            {
                float theta = x * sliceAngle;

                v.pos.x = -height / 2 + radius * cosf(phi);
                v.pos.y = radius * sinf(phi) * cosf(theta);
                v.pos.z = radius * sinf(phi) * sinf(theta);

                v.uv = Vec2(deltaU * x, deltaV * y);

                v.normal.x = sinf(phi) * cosf(theta);
                v.normal.y = cosf(phi);
                v.normal.z = sinf(phi) * sinf(theta);

                v.tangent.x = -radius * sinf(phi) * sinf(theta);
                v.tangent.y = 0.0f;
                v.tangent.z = radius * sinf(phi) * cosf(theta);
                v.tangent.Normalize();

                vec.push_back(v);
            }
        }

        // 남극
        v.pos = Vec3(-height / 2 - radius, 0.0f, 0.0f);
        v.uv = Vec2(0.5f, 1.0f);
        v.normal = v.pos;
        v.normal.Normalize();
        v.tangent = Vec3(1.0f, 0.0f, 0.0f);
        vec.push_back(v);

        // 인덱스 생성
        std::vector<int> idx;

        // 상단 반구 인덱스
        for (int i = 0; i <= sliceCount; ++i)
        {
            idx.push_back(0);
            idx.push_back(i + 2);
            idx.push_back(i + 1);
        }

        // 몸통 인덱스
        int ringVertexCount = sliceCount + 1;
        for (int y = 0; y < stackCount - 2; ++y)
        {
            for (int x = 0; x < sliceCount; ++x)
            {
                idx.push_back(1 + (y)*ringVertexCount + (x));
                idx.push_back(1 + (y)*ringVertexCount + (x + 1));
                idx.push_back(1 + (y + 1) * ringVertexCount + (x));

                idx.push_back(1 + (y + 1) * ringVertexCount + (x));
                idx.push_back(1 + (y)*ringVertexCount + (x + 1));
                idx.push_back(1 + (y + 1) * ringVertexCount + (x + 1));
            }
        }

        // 하단 반구 인덱스
        int bottomIndex = static_cast<int>(vec.size()) - 1;
        int lastRingStartIndex = bottomIndex - ringVertexCount;
        for (int i = 0; i < sliceCount; ++i)
        {
            idx.push_back(bottomIndex);
            idx.push_back(lastRingStartIndex + i);
            idx.push_back(lastRingStartIndex + i + 1);
        }

        shared_ptr<Mesh> pMesh = make_shared<Mesh>();
        pMesh->Initialize(vec, idx);

        return pMesh;
    }
    shared_ptr<Mesh> Resources::CreateTriangleMesh(const wstring& _key, const TriangleMeshInfo& _meshInfo)
    {
        shared_ptr<Mesh> pFindMesh = Get<Mesh>(_key);
        if (pFindMesh)
            return pFindMesh;

        shared_ptr<Mesh> pMesh = make_shared<Mesh>();

        std::vector<Vertex> vertices(_meshInfo.vertices.size());

        for (int i = 0; i < vertices.size(); ++i)
        {
            vertices[i].pos = _meshInfo.vertices[i];
        }

        pMesh->Initialize(vertices, _meshInfo.indices);
        Add(_key, pMesh);
        return pMesh;
    }
    void Resources::CreateDefaultShader()
    {
        // Forward Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Forward,
                DepthStencilType::Less,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\forward.fx", shaderInfo);
            pShader->SetName(L"Forward");
            Add<Shader>(L"Forward", pShader);
        }

        // Deferred Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Deferred,
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"Deferred");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\deferred.fx", shaderInfo);
            Add<Shader>(L"Deferred", pShader);
        }

        // Monster Deferred Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Deferred,
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"MonsterDeferred");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\monster_deferred.fx", shaderInfo);
            Add<Shader>(L"MonsterDeferred", pShader);
        }

        // Compute Test Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Compute,
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"Compute");
            pShader->CreateComputeShader(L"..\\Resources\\Shader\\compute.fx", "CS_Main", "cs_5_0");
            Add<Shader>(L"Compute", pShader);
        }

        // Compute Vertical Blur Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Compute,
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"ComputeVerticalBlur");
            pShader->CreateComputeShader(L"..\\Resources\\Shader\\compute_blur.fx", "CS_VerticalBlur_Main", "cs_5_0");
            Add<Shader>(L"ComputeVerticalBlur", pShader);
        }

        // Compute Horizon Blur Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Compute,
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"ComputeHorizonBlur");
            pShader->CreateComputeShader(L"..\\Resources\\Shader\\compute_blur.fx", "CS_HorizonBlur_Main", "cs_5_0");
            Add<Shader>(L"ComputeHorizonBlur", pShader);
        }

        // Particle
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Forward,
                DepthStencilType::LessNoWrite,
                RasterizerType::CullNone,
                BlendType::AlphaBlend,
                D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST
            };

            ShaderArg shaderArg =
            {
                "VS_Main",
                "GS_Main",
                "PS_Main"
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"Particle");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\particle.fx", shaderInfo, shaderArg);
            Add<Shader>(L"Particle", pShader);
        }

        // Compute Particle
        {
            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"ComputeParticle");
            pShader->CreateComputeShader(L"..\\Resources\\Shader\\particle.fx", "CS_Main", "cs_5_0");
            Add<Shader>(L"ComputeParticle", pShader);
        }

        // DirLight Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullNone,
                BlendType::OneToOneBlend
            };

            ShaderArg shaderArg =
            {
                "VS_DirLight",
                "",
                "PS_DirLight"
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"DirLight");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\lighting.fx", shaderInfo, shaderArg);

            Add<Shader>(L"DirLight", pShader);
        }

        // PointLight Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullFront,
                BlendType::OneToOneBlend
            };

            ShaderArg shaderArg =
            {
                "VS_PointLight",
                "",
                "PS_PointLight"
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"PointLight");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\lighting.fx", shaderInfo, shaderArg);

            Add<Shader>(L"PointLight", pShader);
        }

        // Final Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"Final");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\final.fx", shaderInfo);

            Add<Shader>(L"Final", pShader);
        }

        // Combine Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::Default
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"Combine");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\combine.fx", shaderInfo);

            Add<Shader>(L"Combine", pShader);
        }

        // Combine Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::Default
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"Sampling");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\sampling.fx", shaderInfo);

            Add<Shader>(L"Sampling", pShader);
        }

        // Blur X Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::Default
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"BlurX");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\blur_x.fx", shaderInfo);

            Add<Shader>(L"BlurX", pShader);
        }

        // Blur Y Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::Default
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"BlurY");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\blur_y.fx", shaderInfo);

            Add<Shader>(L"BlurY", pShader);
        }

        // Copy Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::Default
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"Copy");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\copy.fx", shaderInfo);

            Add<Shader>(L"Copy", pShader);
        }

        // Collider Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Forward,
                DepthStencilType::Less,
                RasterizerType::WireFrame,
                BlendType::AlphaBlend,
                D3D_PRIMITIVE_TOPOLOGY_LINELIST
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"Collider");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\collider.fx", shaderInfo);

            Add<Shader>(L"Collider", pShader);
        }

        // Paper Burn Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Forward,
                DepthStencilType::Less,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"PaperBurn");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\paperburn.fx", shaderInfo);

            Add<Shader>(L"PaperBurn", pShader);
        }

        // Paper Burn Deferred Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Deferred,
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"PaperBurn_Deferred");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\paperburn_deferred.fx", shaderInfo);
            Add<Shader>(L"PaperBurn_Deferred", pShader);
        }

        // PlayerSlash Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Forward,
                DepthStencilType::Less,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"PlayerSlash");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\player_slash.fx", shaderInfo);

            Add<Shader>(L"PlayerSlash", pShader);
        }

        // Light Blend Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"LightBlend");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\lightblend.fx", shaderInfo);

            Add<Shader>(L"LightBlend", pShader);
        }

        // DownScale First Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"DownScaleFirst");
            pShader->CreateComputeShader(L"..\\Resources\\Shader\\compute_downscale.fx", "DownScaleFirstPass", "cs_5_0");

            Add<Shader>(L"DownScaleFirst", pShader);
        }

        // DownScale Second Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"DownScaleSecond");
            pShader->CreateComputeShader(L"..\\Resources\\Shader\\compute_downscale.fx", "DownScaleSecondPass", "cs_5_0");

            Add<Shader>(L"DownScaleSecond", pShader);
        }

        // Bloom Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"ComputeBloom");
            pShader->CreateComputeShader(L"..\\Resources\\Shader\\compute_downscale.fx", "BrightPass", "cs_5_0");

            Add<Shader>(L"ComputeBloom", pShader);
        }

        // HDR Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            ShaderArg arg =
            {
                "VS_FullScreenQuad",
                "",
                "PS_FinalPass"
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"HDR");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\hdr.fx", shaderInfo, arg);

            Add<Shader>(L"HDR", pShader);
        }

        // Rim Lighting Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"RimLighting");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\rimlighting.fx", shaderInfo);

            Add<Shader>(L"RimLighting", pShader);
        }

        // Compute Animation
        {
            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"ComputeAnimation");
            pShader->CreateComputeShader(L"..\\Resources\\Shader\\animation.fx", "CS_Main", "cs_5_0");
            Add<Shader>(L"ComputeAnimation", pShader);
        }

        // Compute Light
        {
            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"ComputeLight");
            pShader->CreateComputeShader(L"..\\Resources\\Shader\\compute_light.fx", "CS_Main", "cs_5_0");
            Add<Shader>(L"ComputeLight", pShader);
        }

        // Forward No Culling Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Forward,
                DepthStencilType::Less,
                RasterizerType::CullNone,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"Forward_CullNone");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\forward.fx", shaderInfo);

            Add<Shader>(L"Forward_CullNone", pShader);
        }

        // Deferred No Culling Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Deferred,
                DepthStencilType::Less,
                RasterizerType::CullNone,
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"Deferred_CullNone");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\deferred.fx", shaderInfo);
            Add<Shader>(L"Deferred_CullNone", pShader);
        }

        // DownScale Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"DownScale");
            pShader->CreateComputeShader(L"..\\Resources\\Shader\\downscale.fx", "CS_Main", "cs_5_0");

            Add<Shader>(L"DownScale", pShader);
        }

        // Shadow Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Shadow,
                DepthStencilType::LessEqual,
                RasterizerType::CullNone,
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"Shadow");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\shadow.fx", shaderInfo);
            Add<Shader>(L"Shadow", pShader);
        }

        // Screen Effect Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Light,
                DepthStencilType::NoDepthTestNoWrite,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->SetName(L"ScreenEffect");
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\screeneffect.fx", shaderInfo);

            Add<Shader>(L"ScreenEffect", pShader);
        }

        // No Draw Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Forward,
                DepthStencilType::Less,
                RasterizerType::CullBack,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\no_draw.fx", shaderInfo);
            pShader->SetName(L"NoDraw");
            Add<Shader>(L"NoDraw", pShader);
        }

        // Interface Shader
        {
            ShaderInfo shaderInfo =
            {
                ShaderType::Forward,
                DepthStencilType::NoDepthTest,
                RasterizerType::CullNone,
                BlendType::AlphaBlend
            };

            shared_ptr<Shader> pShader = make_shared<Shader>();
            pShader->CreateGraphicsShader(L"..\\Resources\\Shader\\interface.fx", shaderInfo);
            pShader->SetName(L"Interface");
            Add<Shader>(L"Interface", pShader);
        }
    }
    void Resources::CreateDefaultMaterial()
    {
        // Forward Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Forward");

            pMaterial->SetShader(pShader);
            Add<Material>(L"Forward", pMaterial);
        }

        // Deferred Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Deferred");

            pMaterial->SetShader(pShader);
            Add<Material>(L"Deferred", pMaterial);
        }

        // Monster Deferred Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Deferred");

            pMaterial->SetShader(pShader);
            Add<Material>(L"MonsterDeferred", pMaterial);
        }

        // Compute Test Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Compute");

            pMaterial->SetShader(pShader);
            Add<Material>(L"Compute", pMaterial);
        }

        // Compute Vertical Blur Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"ComputeVerticalBlur");

            pMaterial->SetShader(pShader);
            Add<Material>(L"ComputeVerticalBlur", pMaterial);
        }

        // Compute Horizon Blur Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"ComputeHorizonBlur");

            pMaterial->SetShader(pShader);
            Add<Material>(L"ComputeHorizonBlur", pMaterial);
        }

        // Particle
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Particle");
            pMaterial->SetShader(pShader);
            Add<Material>(L"Particle", pMaterial);
        }

        // Compute Particle
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"ComputeParticle");
            pMaterial->SetShader(pShader);
            Add<Material>(L"ComputeParticle", pMaterial);
        }

        // Dir Light Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"DirLight");

            pMaterial->SetTexture(0, Get<Texture>(L"PositionTarget"));
            pMaterial->SetTexture(1, Get<Texture>(L"NormalTarget"));

            pMaterial->SetShader(pShader);
            Add<Material>(L"DirLight", pMaterial);
        }

        // Point Light Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"PointLight");

            pMaterial->SetTexture(0, Get<Texture>(L"PositionTarget"));
            pMaterial->SetTexture(1, Get<Texture>(L"NormalTarget"));

            pMaterial->SetVec2(0, gpEngine->GetResolution());
            pMaterial->SetShader(pShader);
            Add<Material>(L"PointLight", pMaterial);
        }

        // Final Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Final");

            pMaterial->SetTexture(0, Get<Texture>(L"LightBlendTarget"));

            pMaterial->SetShader(pShader);
            Add<Material>(L"Final", pMaterial);
        }

        // Combine Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Combine");

            pMaterial->SetTexture(0, Get<Texture>(L"SwapChainTarget_0"));
            pMaterial->SetShader(pShader);
            Add<Material>(L"Combine", pMaterial);
        }

        // Sampling Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Sampling");

            pMaterial->SetTexture(0, Get<Texture>(L"SwapChainTarget_0"));
            pMaterial->SetShader(pShader);
            Add<Material>(L"Sampling", pMaterial);
        }

        // Blur X Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"BlurX");

            pMaterial->SetShader(pShader);
            Add<Material>(L"BlurX", pMaterial);
        }

        // Blur Y Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"BlurY");

            pMaterial->SetShader(pShader);
            Add<Material>(L"BlurY", pMaterial);
        }

        // Copy Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Copy");

            pMaterial->SetShader(pShader);
            Add<Material>(L"Copy", pMaterial);
        }

        // Collider Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Collider");

            pMaterial->SetShader(pShader);
            Add<Material>(L"Collider", pMaterial);
        }

        // PaperBurn Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"PaperBurn");
            pMaterial->SetTexture(1, Load<Texture>(L"BurnNoise", L"..\\Resources\\Texture\\BurnNoise.png"));

            pMaterial->SetShader(pShader);
            Add<Material>(L"PaperBurn", pMaterial);
        }

        // PaperBurn Deferred Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"PaperBurn_Deferred");

            pMaterial->SetShader(pShader);
            Add<Material>(L"PaperBurn_Deferred", pMaterial);
        }

        // PlayerSlash Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"PlayerSlash");

            pMaterial->SetShader(pShader);
            Add<Material>(L"PlayerSlash", pMaterial);
        }

        // LightBlend Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"LightBlend");

            pMaterial->SetTexture(0, Get<Texture>(L"DiffuseTarget"));
            pMaterial->SetTexture(1, Get<Texture>(L"DiffuseLightTarget"));
            pMaterial->SetTexture(2, Get<Texture>(L"RimLightingTarget"));

            pMaterial->SetShader(pShader);
            Add<Material>(L"LightBlend", pMaterial);
        }

        // DownScale First Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"DownScaleFirst");

            pMaterial->SetShader(pShader);
            Add<Material>(L"DownScaleFirst", pMaterial);
        }

        // DownScale Second Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"DownScaleSecond");

            pMaterial->SetShader(pShader);
            Add<Material>(L"DownScaleSecond", pMaterial);
        }

        // Compute Bloom Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"ComputeBloom");

            pMaterial->SetShader(pShader);
            Add<Material>(L"ComputeBloom", pMaterial);
        }

        // HDR Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"HDR");

            pMaterial->SetShader(pShader);
            Add<Material>(L"HDR", pMaterial);
        }

        // Rim Lighting Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"RimLighting");

            pMaterial->SetTexture(0, Get<Texture>(L"PositionTarget"));
            pMaterial->SetTexture(1, Get<Texture>(L"NormalTarget"));

            pMaterial->SetShader(pShader);
            Add<Material>(L"RimLighting", pMaterial);
        }

        // Compute Animation Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"ComputeAnimation");

            pMaterial->SetShader(pShader);
            Add<Material>(L"ComputeAnimation", pMaterial);
        }

        // Compute Light Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"ComputeLight");

            pMaterial->SetShader(pShader);
            Add<Material>(L"ComputeLight", pMaterial);
        }

        // Forward No Culling Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Forward_CullNone");

            pMaterial->SetShader(pShader);
            Add<Material>(L"Forward_CullNone", pMaterial);
        }

        // Deferred No Culling Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Deferred_CullNone");

            pMaterial->SetShader(pShader);
            Add<Material>(L"Deferred_CullNone", pMaterial);
        }

        // DownScale Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"DownScale");
            pMaterial->SetTexture(0, Get<Texture>(L"BloomTarget"));

            pMaterial->SetShader(pShader);
            Add<Material>(L"DownScale", pMaterial);
        }

        // Shadow Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Shadow");

            pMaterial->SetShader(pShader);
            Add<Material>(L"Shadow", pMaterial);
        }

        // ScreenEffect Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"ScreenEffect");

            pMaterial->SetShader(pShader);
            Add<Material>(L"ScreenEffect", pMaterial);
        }

        // No Draw Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"NoDraw");

            pMaterial->SetShader(pShader);
            Add<Material>(L"NoDraw", pMaterial);
        }

        // UI Material
        {
            shared_ptr<Material> pMaterial = make_shared<Material>();
            shared_ptr<Shader> pShader = Get<Shader>(L"Interface");
            pMaterial->ClearMaterialContainers();

            pMaterial->SetShader(pShader);
            Add<Material>(L"Interface", pMaterial);
        }
    }
}