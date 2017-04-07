// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

/**
 * Implementation of the Park Miller (1988) "minimal standard" linear
 * congruential pseudo-random number generator.
 *
 * For a full explanation visit: http://www.firstpr.com.au/dsp/rand31/
 *
 * The generator uses a modulus constant (m) of 2^31 - 1 which is a
 * Mersenne Prime number and a full-period-multiplier of 16807.
 * Output is a 31 bit unsigned integer. The range of values output is
 * 1 to 2,147,483,646 (2^31-1) and the seed must be in this range too.
 *
 * David G. Carta's optimisation which needs only 32 bit integer math,
 * and no division is actually *slower* in flash (both AS2 & AS3) so
 * it's better to use the double-precision floating point version.
 *
 * @author Michael Baczynski, www.polygonal.de
 */
class PM_PRNG
{
public:
    PM_PRNG(int seed)
    {
        m_unSeed = seed;
    }
    ~PM_PRNG(){};
    
    unsigned int nextInt() { return gen(); }
    double nextDouble() { return (gen() / 2147483647.0); }
    unsigned int nextIntRange(double min, double max)
    {
        min -= .4999;
        max += .4999;
        return round(min + ((max - min) * nextDouble()));
    }
    double nextDoubleRange(double min, double max)
    {
        return min + ((max - min) * nextDouble());
    }
    unsigned int gen()
    {
        //integer version 1, for max int 2^46 - 1 or larger.
        return m_unSeed = (m_unSeed * 16807) % 2147483647;
        
        /**
         * integer version 2, for max int 2^31 - 1 (slowest)
         */
        //int test = 16807 * (m_unSeed % 127773 >> 0) - 2836 * (m_unSeed / 127773 >> 0);
        //return m_unSeed = (test > 0 ? test : test + 2147483647);
        
        /**
         * david g. carta's optimisation is 15% slower than integer version 1
         */
        //unsigned int hi = 16807 * (m_unSeed >> 16);
        //unsigned int lo = 16807 * (m_unSeed & 0xFFFF) + ((hi & 0x7FFF) << 16) + (hi >> 15);
        //return m_unSeed = (lo > 0x7FFFFFFF ? lo - 0x7FFFFFFF : lo);
    }
private:
    unsigned int m_unSeed;
};

UENUM(BlueprintType)
enum class EIslandShapeType : uint8
{
    ISTE_Radial		UMETA(DisplayName = "Radial"),
    ISTE_Perlin		UMETA(DisplayName = "Perlin"),
    ISTE_Square		UMETA(DisplayName = "Square"),
    ISTE_Blob		UMETA(DisplayName = "Blob")
};

UENUM(BlueprintType)
enum class EPointType : uint8
{
    PTE_Random		UMETA(DisplayName = "Random"),
    PTE_Relaxed		UMETA(DisplayName = "Relaxed"),
    PTE_Square		UMETA(DisplayName = "Square"),
    PTE_Hexagon		UMETA(DisplayName = "Hexagon")
};

class IslandMap;
/**
 * 岛屿生成器
 */
class IslandGenerator
{
public:
	IslandGenerator();
	~IslandGenerator();
    
    static IslandGenerator* getInstance();
    static void releaseInstance();
    
    bool generate(int size, EIslandShapeType islandShapeType, EPointType pointType, int numPoints);
private:
    float m_fIslandFactor; // 1.0 means no small islands; 2.0 leads to a lot
    
    IslandMap*  m_pIslandMap;
};
