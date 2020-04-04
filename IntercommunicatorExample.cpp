#include <iostream>
#include "MPILib.h"

int main(int argc, char* argv[]) {

    auto worldCommunicator = MPI::getInst().getWorldComm();
    
    if (worldCommunicator.getSize() != 9)
    {
        std::cout << "Number of processes must be 9! Exiting..";
        return 0;
    }

    constexpr int SERVER_RANK = 0;
    int worldRank = worldCommunicator.getRank();
    bool isServer = (worldRank == SERVER_RANK);

    // Split all processes into two subgroups: server and the rest
    auto localComm = worldCommunicator.split(isServer, worldRank);

    constexpr int TAG_FOR_1ST_INTERCOMM = 0;
    constexpr int TAG_FOR_2ND_INTERCOMM = 1;
    if (!isServer)
    {
        auto localGroup = localComm.getGroup();
        auto group1Comm = MPICommunicator::makeBasedOnGroup(MPIGroup::makeByInclude(localGroup, { 0, 1, 2, 3 }), localComm);
        auto group2Comm = MPICommunicator::makeBasedOnGroup(MPIGroup::makeByInclude(localGroup, { 4, 5, 6, 7 }), localComm);

        // One intercommunicator for each group to communicate with server
        MPICommunicator interComm;
        if (!group1Comm.isNull())
        {
            int localLeaderRank = 0;
            MPICommunicator::makeInterCommunicator(group1Comm, localLeaderRank, worldCommunicator, SERVER_RANK, TAG_FOR_1ST_INTERCOMM, interComm);
            if (group1Comm.getRank() == localLeaderRank)
            {
                std::cout << "Local leader of 1st group is sending data to server by intercommunicator...\n";
                interComm.sendArray({1, 2, 3}, 0);
            }
        }
        if (!group2Comm.isNull())
        {
            int localLeaderRank = 0;
            MPICommunicator::makeInterCommunicator(group2Comm, localLeaderRank, worldCommunicator, SERVER_RANK, TAG_FOR_2ND_INTERCOMM, interComm);
            if (group2Comm.getRank() == localLeaderRank)
            {
                std::cout << "Local leader of 2nd group is sending data to server by intercommunicator...\n";
                interComm.send(888, 0);
            }
        }
    }
    else
    {
        int localLeaderRank = 0;
        int remoteLeaderRank;
        MPICommunicator interCommWith1stGroup;
        MPICommunicator::makeInterCommunicator(localComm, localLeaderRank, worldCommunicator, (remoteLeaderRank = 1), TAG_FOR_1ST_INTERCOMM, interCommWith1stGroup);
        MPICommunicator interCommWith2ndGroup; 
        MPICommunicator::makeInterCommunicator(localComm, localLeaderRank, worldCommunicator, (remoteLeaderRank = 5), TAG_FOR_2ND_INTERCOMM, interCommWith2ndGroup);

        std::cout << "Server has received array with size " << interCommWith1stGroup.receiveArray(0).size() << " from 1st group\n";
        std::cout << "Server has received " << interCommWith2ndGroup.receive(0) << " from 2nd group\n";
    }

    return 0;
}