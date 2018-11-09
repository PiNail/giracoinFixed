// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Giracoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "chainparamsseeds.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>
#include <stdio.h>

#include <boost/assign/list_of.hpp>

using namespace std;

/**
 * Main network
 */
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 0; // no halving, it is a no-op
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = ArithToUint256(~arith_uint256(0) >> 20);
        consensus.nPowTargetSpacing = 2 * 60; // 2 mins
        consensus.nPowTargetTimespan = 1 * 60 * 60; // 1 hour
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nMinerConfirmationWindow = 30; // nPowTargetTimespan / nPowTargetSpacing
        consensus.nRuleChangeActivationThreshold = 30; // nPowTargetTimespan / nPowTargetSpacing

        /** [PoS implementation] */
        consensus.nPosTargetSpacing = 1 * 60; // 1 min
        consensus.nPosTargetTimespan = 30 * 60; // 30 mins
        consensus.nStakeMinConfirmations = 100; // == COINBASE_MATURITY
        consensus.posLimit = ArithToUint256(~arith_uint256(0) >> 20);

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1462060800; // May 1st, 2016
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1493596800; // May 1st, 2017

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1479168000; // November 15th, 2016.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1510704000; // November 15th, 2017.

        consensus.nMinimumChainWork = uint256S("000000000000000000000000000000000000000000000000000000001e00ffff"); // same as minDiff in our case
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
        */
        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        nDefaultPort = 2016;
        nPruneAfterHeight = 1000000000;

        const char* hashMerkleRoot   = "0x035aa7b0db431043afd8f6e1a52c698c1abbdb3f09bf84b2b824e1f4918806d1";
        const char* hashGenesisBlock = "0x00000cbb83829393ca10bc123d3606dbea90672d75b26e5acd2fe46a2e599935";
		const char* pszTimestamp = "Giracoin Test Blockchain, developed by NovaLab (novalab.io)";
		CMutableTransaction txNew;
		txNew.vin.resize(1);
		txNew.vout.resize(1);
		txNew.vin[0].scriptSig = CScript() << 232346463 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 2600 * COIN; // not spendable
		txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;

        genesis.nVersion = 1;
        genesis.nTime    = 1492806028;
        genesis.nBits    = 0x1e0ffff0;
        genesis.nNonce   = 434955;
		genesis.vtx.push_back(txNew);
		genesis.hashPrevBlock.SetNull();
		genesis.hashMerkleRoot = BlockMerkleRoot(genesis);

		// create genesis block
		arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);

		if (genesis.GetHash() != uint256S(hashGenesisBlock)) {
			while (UintToArith256(genesis.GetHash()) > hashTarget) {
				++genesis.nNonce;
                printf("block.nNonce = %u \n", genesis.nNonce);
				if (genesis.nNonce == 0) {
					LogPrintf("NONCE WRAPPED, incrementing time");
					++genesis.nTime;
				}
			}

			printf("block.GetHash() = %s \n", genesis.GetHash().ToString().c_str());
			printf("block.hashMerkleRoot = %s \n", genesis.hashMerkleRoot.ToString().c_str());
            printf("block.nBits = %u \n", genesis.nBits);
			printf("block.nTime = %u \n", genesis.nTime);
			printf("block.nNonce = %u \n", genesis.nNonce);
		}

        consensus.hashGenesisBlock = genesis.GetHash();

        assert(genesis.hashMerkleRoot == uint256S(hashMerkleRoot));
        assert(consensus.hashGenesisBlock == uint256S(hashGenesisBlock));

        vSeeds.push_back(CDNSSeedData("seed1", "seed1.giracoin.com"));
        vSeeds.push_back(CDNSSeedData("seed2", "seed2.giracoin.com"));
        vSeeds.push_back(CDNSSeedData("seed3", "seed3.giracoin.com"));
        vSeeds.push_back(CDNSSeedData("seed4", "seed4.giracoin.com"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,38);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S(hashGenesisBlock)),
            1492806028, // * UNIX timestamp of last checkpoint block
            0,          // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            2160.0      // * estimated number of transactions per day after checkpoint
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nMajorityEnforceBlockUpgrade = 51;
		consensus.nMajorityRejectBlockOutdated = 75;
		consensus.nMajorityWindow = 100;
		consensus.fPowAllowMinDifficultyBlocks = true;

        pchMessageStart[0] = 0x0b;
        pchMessageStart[1] = 0x11;
        pchMessageStart[2] = 0x09;
        pchMessageStart[3] = 0x07;
        //vAlertPubKey = ParseHex("04302390343f91cc401d56d68b123028bf52e5fca1939df127f63c6467cdf9c8e2c14b61104cf817d0b780da337893ecc4aaff1309e536162dabbdb45200ca2b0a");
		nDefaultPort = 18333;
		//nMinerThreads = 0;
		//nMaxTipAge = 0x7fffffff;
		nPruneAfterHeight = 1000;

		//! Modify the testnet genesis block so the timestamp is valid for a later start.
		genesis.nTime = 1474894940;
		genesis.nNonce = 9077257;
		consensus.hashGenesisBlock = genesis.GetHash();
		//assert(consensus.hashGenesisBlock == uint256S("0x000000da91c21637a6450bb29d97f8e94075d6c17716f221980a4e62a988607b"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        //fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
			( 0, uint256S("000000da91c21637a6450bb29d97f8e94075d6c17716f221980a4e62a988607b")),
            1337966069,
            1488,
            300
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        //nMinerThreads = 1;
		//nMaxTipAge = 24 * 60 * 60;
		genesis.nTime = 1474894940;
		genesis.nBits = 0x207fffff;
		genesis.nNonce = 9077257;
		consensus.hashGenesisBlock = genesis.GetHash();
		nDefaultPort = 18444;
		//assert(consensus.hashGenesisBlock == uint256S("0x000000da91c21637a6450bb29d97f8e94075d6c17716f221980a4e62a988607b"));
		nPruneAfterHeight = 1000;

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear(); //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        checkpointData = (CCheckpointData) {
			boost::assign::map_list_of
			( 0, uint256S("000000da91c21637a6450bb29d97f8e94075d6c17716f221980a4e62a988607b")),
			0,
			0,
			0
		};
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
 
