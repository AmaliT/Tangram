/*
 * =====================================================================================
 *
 *       Filename:  TGM_Genotype.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/21/2012 11:55:55 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jiantao Wu (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  TGM_GENOTYPE_H
#define  TGM_GENOTYPE_H

#include "TGM_Array.h"
#include "TGM_Detector.h"
#include "TGM_SplitData.h"
#include "TGM_LibTable.h"

#include "api/BamMultiReader.h"

namespace Tangram
{
    // fragment count for each sample
    struct FragCount
    {
        uint32_t nonSupport;

        uint32_t support;

        uint32_t rp3;

        uint32_t rp5;

        uint32_t sr3;

        uint32_t sr5;
    };

    class Genotype
    {
        public:

            Genotype(BamTools::BamMultiReader& reader, const GenotypePars& genotypePars, const LibTable& libTable, BamPairTable& bamPairTable);

            ~Genotype();

            void Init(void);

            // set the prior probabilities for MEI insertions
            void SetSpecialPrior(const double* prior);

            // do the genotype for a special insertion locus
            bool Special(const SpecialEvent* pRpSepcial, const SplitEvent* pSplitEvent);

        private:

            bool SpecialFilter(const SpecialEvent* pRpSpecial, const SplitEvent* pSplitEvent) const;

            // jump to a specific position in the bam file
            bool Jump(int32_t refID, int32_t pos);

            // set the read-pair fragment count for each sample
            void SetSampleCountSpecial(const SpecialEvent& rpSpecial);

            // set the split-read fragment count for each sample
            void SetSampleCountSplit(const SplitEvent& splitEvent);

            // update the number of non-supporting fragment for a given sample
            inline void UpdateNonSupport(int32_t readGrpID)
            {
                unsigned int sampleID = 0;
                if (libTable.GetSampleID(sampleID, readGrpID))
                    ++(sampleCount[sampleID].nonSupport);
            }

            // update the number of supporting fragment for a given sample
            inline void UpdateSupport(int32_t readGrpID)
            {
                unsigned int sampleID = 0;
                if (libTable.GetSampleID(sampleID, readGrpID))
                    ++(sampleCount[sampleID].support);
            }

            void SetLikelihood(void);

            // calculate the log10 of N choose K value
            long double Log10NchooseK(unsigned int n, unsigned int k) const;

            // assume diploid genome.
            // binomial pdf
            double CalculateLikelihood(long double log10CNM, unsigned int refCount, unsigned int altCount, double p) const;


        public:

            // genotype vector
            std::vector<int8_t> genotypes;

            // likelihood vector (3 elements for one sample)
            std::vector<double> likelihoods;

            // fragment count vector
            Array<FragCount> sampleCount;

        private:

            BamTools::BamMultiReader& reader;

            // genotyping parameters
            const GenotypePars& genotypePars;

            const LibTable& libTable;

            BamPairTable& bamPairTable;

            // used to keep track of the current stream position
            int32_t lastChr;
            int32_t lastPos;
            int32_t lastEnd;

            // prior probabilities for MEI insertions
            double specialPrior[3];
    };
};

#endif  /*TGM_GENOTYPE_H*/
