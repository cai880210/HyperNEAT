#include "HCUBE_Defines.h"

#include "Experiments/HCUBE_CheckersExperimentFogel.h"

#ifdef EPLEX_INTERNAL

#define CHECKERS_EXPERIMENT_DEBUG (0)

namespace HCUBE
{
    using namespace NEAT;

    CheckersExperimentFogel::CheckersExperimentFogel(string _experimentName,int _threadID)
            :
            CheckersExperiment(_experimentName,_threadID)
    {
        generateSubstrate();
    }

    GeneticPopulation* CheckersExperimentFogel::createInitialPopulation(int populationSize)
    {
        GeneticPopulation *population = new GeneticPopulation();
        vector<GeneticNodeGene> genes;

        genes.push_back(GeneticNodeGene("Bias","NetworkSensor",0,false));

        //genes.push_back(GeneticNodeGene("InputVectorSum","NetworkSensor",0,false));

        for (int x=0;x<8;x++)
        {
            for (int y=0;y<8;y++)
            {
                if (x<6&&y<6)
                {
                    string nodeName = string("Input_3x3_") + toString(x) + string("_") + toString(y);
                    genes.push_back(GeneticNodeGene(nodeName,"NetworkSensor",0,false));
                }

                if (x<5&&y<5)
                {
                    string nodeName = string("Input_4x4_") + toString(x) + string("_") + toString(y);
                    genes.push_back(GeneticNodeGene(nodeName,"NetworkSensor",0,false));
                }

                if (x<4&&y<4)
                {
                    string nodeName = string("Input_5x5_") + toString(x) + string("_") + toString(y);
                    genes.push_back(GeneticNodeGene(nodeName,"NetworkSensor",0,false));
                }

                if (x<3&&y<3)
                {
                    string nodeName = string("Input_6x6_") + toString(x) + string("_") + toString(y);
                    genes.push_back(GeneticNodeGene(nodeName,"NetworkSensor",0,false));
                }

                if (x<2&&y<2)
                {
                    string nodeName = string("Input_7x7_") + toString(x) + string("_") + toString(y);
                    genes.push_back(GeneticNodeGene(nodeName,"NetworkSensor",0,false));
                }

                if (x<1&&y<1)
                {
                    string nodeName = string("Input_8x8_") + toString(x) + string("_") + toString(y);
                    genes.push_back(GeneticNodeGene(nodeName,"NetworkSensor",0,false));
                }
            }
        }

        for (int x=0;x<8;x++)
        {
            for (int y=0;y<8;y++)
            {
                if ((x+y)%2==0)
                {
                    Node node(x,y,0);
                    //cout << (y1-numNodesY/2) << '/' << (x1-numNodesX/2) << endl;
                    string name = (toString(x)+string("/")+toString(y) + string("/") + toString(0));
                    nameLookup[node] = name;
                    genes.push_back(GeneticNodeGene(name,"NetworkSensor",0,false));
                }
            }
        }

        Node node(0,0,2);
        //cout << (y1-numNodesY/2) << '/' << (x1-numNodesX/2) << endl;
        string name = (toString(0)+string("/")+toString(0) + string("/") + toString(2));
        nameLookup[node] = name;
        genes.push_back(GeneticNodeGene(name,"NetworkOutputNode",1,false,ACTIVATION_FUNCTION_SIGMOID));

        for (int a=0;a<populationSize;a++)
        {
            shared_ptr<GeneticIndividual> individual(new GeneticIndividual(genes,true,1.0));

            for (int b=0;b<0;b++)
            {
                individual->testMutate();
            }

            population->addIndividual(individual);
        }

        cout << "Finished creating population\n";
        return population;
    }

    void CheckersExperimentFogel::generateSubstrate(int substrateNum)
    {}

    void CheckersExperimentFogel::populateSubstrate(
        shared_ptr<NEAT::GeneticIndividual> individual,
        int substrateNum
    )
    {
        if (substrateIndividuals[substrateNum]==individual)
        {
            //Don't bother remaking the same substrate
            return;
        }

        substrateIndividuals[substrateNum]=individual;

        //Clear the evaluation cache
        for (int a=0;a<65536;a++)
        {
            boardEvaluationCaches[substrateNum][a].clear();
        }

        //Clear the state list caches
        for (int a=0;a<65536;a++)
        {
            boardStateLists[substrateNum][BLACK][a].clear();
        }
        for (int a=0;a<65536;a++)
        {
            boardStateLists[substrateNum][WHITE][a].clear();
        }

        networks[substrateNum] = individual->spawnFastPhenotypeStack<CheckersNEATDatatype>();
    }

    pair<CheckersNEATDatatype,int> CheckersExperimentFogel::evaluateLeafHyperNEAT(uchar b[8][8])
    {
#if CHECKERS_EXPERIMENT_ENABLE_BIASES
        NEAT::FastBiasNetwork<CheckersNEATDatatype>* substrate;

        throw CREATE_LOCATEDEXCEPTION_INFO("ERROR: Can't run NoGeom version of CheckersExperiment with biases turned on!");
#else
        NEAT::FastNetwork<CheckersNEATDatatype>* substrate;
#endif

        substrate = &networks[currentSubstrateIndex];

        substrate->reinitialize();

        if (substrate->hasNode("Bias"))
        {
            substrate->setValue("Bias",(CheckersNEATDatatype)0.3);
        }

        for (int x=0;x<8;x++)
        {
            for (int y=0;y<8;y++)
            {
                if (x<6&&y<6)
                {
                    string nodeName = string("Input_3x3_") + toString(x) + string("_") + toString(y);
                    substrate->setValue(nodeName,getSpatialInput(b,x,y,3,3));
                }

                if (x<5&&y<5)
                {
                    string nodeName = string("Input_4x4_") + toString(x) + string("_") + toString(y);
                    substrate->setValue(nodeName,getSpatialInput(b,x,y,4,4));
                }

                if (x<4&&y<4)
                {
                    string nodeName = string("Input_5x5_") + toString(x) + string("_") + toString(y);
                    substrate->setValue(nodeName,getSpatialInput(b,x,y,5,5));
                }

                if (x<3&&y<3)
                {
                    string nodeName = string("Input_6x6_") + toString(x) + string("_") + toString(y);
                    substrate->setValue(nodeName,getSpatialInput(b,x,y,6,6));
                }

                if (x<2&&y<2)
                {
                    string nodeName = string("Input_7x7_") + toString(x) + string("_") + toString(y);
                    substrate->setValue(nodeName,getSpatialInput(b,x,y,7,7));
                }

                if (x<1&&y<1)
                {
                    string nodeName = string("Input_8x8_") + toString(x) + string("_") + toString(y);
                    substrate->setValue(nodeName,getSpatialInput(b,x,y,8,8));
                }
            }
        }

        substrate->update();
        double output = substrate->getValue(getNameFromNode(Node(0,0,2)));

        static double prevOutput;


#if CHECKERS_EXPERIMENT_DEBUG
        if (fabs(output-prevOutput)>1e-3)
        {
            prevOutput = output;
            cout << "BOARD RATING:" << output << endl;
            CREATE_PAUSE("");
        }
#endif
        return pair<CheckersNEATDatatype,int>(output,-1);
    }

    CheckersNEATDatatype CheckersExperimentFogel::getSpatialInput(uchar b[8][8],int x,int y,int sizex,int sizey)
    {
        CheckersNEATDatatype sum=0.0;
        for (int boardx=x;boardx<x+sizex;boardx++)
        {
            for (int boardy=y;boardy<y+sizey;boardy++)
            {
                if ( (boardx+boardy)%2==1 ) //ignore empty squares.
                    continue;

                if ( (b[boardx][boardy]&WHITE) )
                {
                    //cout << "FOUND WHITE\n";
                    if ( (b[boardx][boardy]&KING) )
                    {
                        sum -= (CheckersNEATDatatype)1.3;
                    }
                    else if ( (b[boardx][boardy]&MAN) )
                    {
                        sum -= (CheckersNEATDatatype)1.0;
                    }
                    else
                    {
                        throw CREATE_LOCATEDEXCEPTION_INFO("Could not determine piece type (man/king)!");
                    }
                }
                else if ( (b[boardx][boardy]&BLACK) )
                {
                    //cout << "FOUND BLACK\n";
                    if ( (b[boardx][boardy]&KING) )
                    {
                        sum += (CheckersNEATDatatype)1.3;
                    }
                    else if ( (b[boardx][boardy]&MAN) )
                    {
                        sum += (CheckersNEATDatatype)1.0;
                    }
                    else
                    {
                        throw CREATE_LOCATEDEXCEPTION_INFO("Could not determine piece type (man/king)!");
                    }
                }
                else
                {
                }

            }
        }

        return (2.0 / (1 + exp(-1.0 * sum)) - 1.0);
    }

    Experiment* CheckersExperimentFogel::clone()
    {
        CheckersExperimentFogel* experiment = new CheckersExperimentFogel(*this);

        return experiment;
    }
}

#endif
