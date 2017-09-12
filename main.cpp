
#include <iostream>
#include <map>
#include <sys/shm.h>

using namespace std;

struct NFMemoryGUID
{
    int64_t d1;
    int64_t d2;
};

struct NFMemoryVector2
{
    float x;
    float y;
};

struct NFMemoryVector3
{
    float x;
    float y;
    float z;
};

struct NFMemoryString
{
    void* p;
    int32_t length;
};

struct NFMemoryBase//int,float,double,vector2,vector3
{
    NFMemoryGUID id;  //16bytes--user
    int16_t proertyID; //2byte---crc
    int8_t type;  //1byte
    union value //16bytes
    {
        int64_t n;
        float f;
        double dw;
        NFMemoryVector2 v2;
        NFMemoryVector3 v3;
        NFMemoryGUID id;
        NFMemoryString str;//for string, 64bytes,memory address, 64bytes, string length
    };
};

class NFMemoryAlloc
{
public:
    NFMemoryAlloc()
    {
        InitBaseMemory();
        InitStringMemory();
        rebuildMemory();
    }

    bool InitBaseMemory()
    {
        int shmid = shmget(1234, sizeof(NFMemoryBase)*1024, 0666|IPC_CREAT);
        if(shmid == -1)
        {
            fprintf(stderr, "shmget failed\n");
        }

        shm1 = shmat(shmid, 0, 0);
        if(shm1 == (void*)-1)
        {
            fprintf(stderr, "shmat failed\n");
        }

        printf("\nMemory attached at %X\n", shmid);


        //读取
    }

    bool InitStringMemory()
    {

    }

    NFMemoryBase* allocMemory(NFMemoryGUID id, int16_t propertyID, int8_t type)
    {
        int64_t address =int64_t(shm1) + index * size;
        NFMemoryBase* p = (NFMemoryBase*)(address);

        ++index;

        p->id = id;
        p->proertyID = propertyID;
        p->type = type;

        return p;
    }

    void releaseMemory(NFMemoryBase* p)
    {
        memset(p, 0, sizeof(NFMemoryBase));
    }

    //only can be used after rebuid memory
    bool rebuildMemory()
    {
        for (int i = 0; i < count; ++i)
        {
            int64_t address =int64_t(shm1) + i * size;
            NFMemoryBase* p = (NFMemoryBase*)(address);
            //if (*p != 0)
            {
                //mxReferenceData.find(p->id);
            }
        }

        return true;
    }

    NFMemoryBase* findMemory(NFMemoryGUID id, int16_t propertyID)
    {
        return NULL;
    }

    void clearReference()
    {
        //mxReferenceData.clear();
    }

private:
//    std::map< NFGUID, < std::map< int, MemoryBase* > > > mxReferenceData;

private:
    int index = 0;
    int size = 35;
    int count = 10000;

    void* shm1 = NULL;//分配的共享内存的原始首地址--base
    void* shm2 = NULL;//分配的共享内存的原始首地址--string
};

int main()
{
    NFMemoryAlloc memoryAlloc;
    for (int i = 0; i < 6; ++i)
    {
        NFMemoryGUID id;
        id.d1 = 0;
        id.d2 = i;

        NFMemoryBase* pMemory = memoryAlloc.allocMemory(id, i, 1);

        std::cout << pMemory->id.d2 << endl;
        std::cout << pMemory->type << endl;

    }

    cout << "Hello world!" << endl;
    return 0;
}
