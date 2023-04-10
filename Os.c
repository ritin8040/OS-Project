#include<bits/stdc++.h>

class MemoryManager{
    
    int memorySize;
    int allocatedMemory;
    int numProcesses;
    int numWastedBlocks;
    int numFragments;
    std::list<int> freeList;
    std::map<int, int> processMap;

    public:
        MemoryManager(int memorySize){
            this->memorySize = memorySize;
            allocatedMemory = 0;
            numProcesses = 0;
            numWastedBlocks = 0;
            numFragments = 0;
            freeList.push_back(0);
        }
    
        bool allocateMemory(int processId, int processSize){
            if (processSize > (memorySize - allocatedMemory))
            {
                ++numWastedBlocks;
                return false;
            }
    
            std::list<int>::iterator it = freeList.begin();
            while (it != freeList.end())
            {
                int startIndex = *it;
                int endIndex = startIndex + processSize - 1;
                bool isFree = true;
                if (endIndex >= memorySize)
                {
                    isFree = false;
                    break;
                    // return false;
                }
                if (isFree)
                {
                    it = freeList.erase(it);
                    for (int i = startIndex; i <= endIndex; i++)
                    {
                        processMap[i] = processId;
                    }
                    allocatedMemory += processSize;
                    numProcesses++;
                    
                    return true;
                }
                else
                {
                    ++it;
                }
            }
    
            
            return false;
        }
    
        void deallocateMemory(int processId){
            std::list<int> blocksToFree;
            for (auto it = processMap.begin(); it != processMap.end(); ++it)
            {
                if (it->second == processId)
                {
                    blocksToFree.push_back(it->first);
                }
            }
            for (auto block : blocksToFree)
            {
                processMap.erase(block);
                freeList.push_back(block);
            }
            numProcesses--;
            allocatedMemory -= blocksToFree.size();
        }
    
        void runSimulation(){
            
                numFragments = freeList.size();
                
                std::cout<<"----------------------"<<std::endl;
                std::cout << "No. of Processes: " << numProcesses << std::endl;
                std::cout << "No. of Wasted Blocks: " << numWastedBlocks << std::endl;
                std::cout << "No. of Fragments: " << numFragments <<std:: endl;
                std::cout << "Allocated Memory: " << allocatedMemory << "/" << memorySize << std::endl;
        }
};

int main(){
    MemoryManager memoryManager(100);
    memoryManager.allocateMemory(1, 20);
    memoryManager.runSimulation();
    memoryManager.deallocateMemory(1);
    memoryManager.runSimulation();
    memoryManager.allocateMemory(2, 30);
    memoryManager.runSimulation();
    memoryManager.allocateMemory(2, 90);
    memoryManager.runSimulation();
    return 0;
}
