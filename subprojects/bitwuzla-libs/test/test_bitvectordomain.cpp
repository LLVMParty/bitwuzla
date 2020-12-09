#include <bitset>

#include "bitvector_domain.h"
#include "gmpmpz.h"
#include "gtest/gtest.h"

namespace bzlals {

class TestBitVectorDomain : public ::testing::Test
{
 protected:
  void test_match_fixed_bits(const std::string& value);
};

void
TestBitVectorDomain::test_match_fixed_bits(const std::string& value)
{
  assert(value.size() == 3);
  BitVectorDomain d(value);
  for (uint32_t i = 0; i < (1u << 3); ++i)
  {
    bool expected      = true;
    std::string bv_val = std::bitset<3>(i).to_string();
    BitVector bv(3, bv_val);
    for (uint32_t j = 0; j < 3; ++j)
    {
      if ((d.is_fixed_bit_false(j) && bv_val[2 - j] != '0')
          || (d.is_fixed_bit_true(j) && bv_val[2 - j] != '1'))
      {
        expected = false;
      }
    }
    ASSERT_EQ(d.match_fixed_bits(bv), expected);
  }
}

TEST_F(TestBitVectorDomain, ctor_dtor)
{
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(1));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(10));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(BitVector(10, 4), BitVector(10, 5)));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain("00000000"));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain("11111111"));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain("10110100"));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain("x01xxxxx"));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain("xxxxxxxx"));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(BitVector(4, "0000")));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(BitVector(8, "1010")));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(4, 0));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(8, 5));
  ASSERT_NO_FATAL_FAILURE(BitVectorDomain(BitVectorDomain(8, 5)));
  ASSERT_DEATH(BitVectorDomain(0), "size > 0");
  ASSERT_DEATH(BitVectorDomain(BitVector(10, 4), BitVector(11, 5)),
               "lo.get_size\\(\\) == hi.get_size\\(\\)");
  ASSERT_DEATH(BitVectorDomain(""), "size > 0");
  ASSERT_DEATH(BitVectorDomain("12345"), "is_bin_str");
}

TEST_F(TestBitVectorDomain, get_size)
{
  ASSERT_EQ(BitVectorDomain(1).get_size(), 1);
  ASSERT_EQ(BitVectorDomain(10).get_size(), 10);
  ASSERT_EQ(BitVectorDomain(BitVector(10, 4), BitVector(10, 5)).get_size(), 10);
  ASSERT_EQ(BitVectorDomain("00000000").get_size(), 8);
  ASSERT_EQ(BitVectorDomain("11111111").get_size(), 8);
  ASSERT_EQ(BitVectorDomain("10110100").get_size(), 8);
  ASSERT_EQ(BitVectorDomain("x01xxxxx").get_size(), 8);
  ASSERT_EQ(BitVectorDomain("xxxxxxxx").get_size(), 8);
  ASSERT_EQ(BitVectorDomain(BitVector(4, "0000")).get_size(), 4);
  ASSERT_EQ(BitVectorDomain(BitVector(8, "1010")).get_size(), 8);
  ASSERT_EQ(BitVectorDomain(4, 0).get_size(), 4);
  ASSERT_EQ(BitVectorDomain(8, 5).get_size(), 8);
  ASSERT_EQ(BitVectorDomain(BitVectorDomain(8, 5)).get_size(), 8);
}

TEST_F(TestBitVectorDomain, is_valid)
{
  ASSERT_TRUE(BitVectorDomain(1).is_valid());
  ASSERT_TRUE(BitVectorDomain(10).is_valid());
  ASSERT_TRUE(BitVectorDomain(BitVector(10, 4), BitVector(10, 5)).is_valid());
  ASSERT_TRUE(BitVectorDomain("00000000").is_valid());
  ASSERT_TRUE(BitVectorDomain("11111111").is_valid());
  ASSERT_TRUE(BitVectorDomain("10110100").is_valid());
  ASSERT_TRUE(BitVectorDomain("x01xxxxx").is_valid());
  ASSERT_TRUE(BitVectorDomain("xxxxxxxx").is_valid());
  ASSERT_TRUE(BitVectorDomain(BitVector(4, "0000")).is_valid());
  ASSERT_TRUE(BitVectorDomain(BitVector(8, "1010")).is_valid());
  ASSERT_TRUE(BitVectorDomain(4, 0).is_valid());
  ASSERT_TRUE(BitVectorDomain(8, 5).is_valid());
  ASSERT_TRUE(BitVectorDomain(BitVectorDomain(8, 5)).is_valid());
  ASSERT_FALSE(
      BitVectorDomain(BitVector(4, "1000"), BitVector(4, "0111")).is_valid());
  ASSERT_FALSE(
      BitVectorDomain(BitVector(4, "0100"), BitVector(4, "1011")).is_valid());
  ASSERT_FALSE(
      BitVectorDomain(BitVector(4, "0110"), BitVector(4, "1001")).is_valid());
  ASSERT_FALSE(
      BitVectorDomain(BitVector(4, "1001"), BitVector(4, "0110")).is_valid());
}

TEST_F(TestBitVectorDomain, is_fixed)
{
  ASSERT_FALSE(BitVectorDomain(1).is_fixed());
  ASSERT_FALSE(BitVectorDomain(10).is_fixed());
  ASSERT_FALSE(BitVectorDomain(BitVector(10, 4), BitVector(10, 5)).is_fixed());
  ASSERT_TRUE(BitVectorDomain("00000000").is_fixed());
  ASSERT_TRUE(BitVectorDomain("11111111").is_fixed());
  ASSERT_TRUE(BitVectorDomain("10110100").is_fixed());
  ASSERT_FALSE(BitVectorDomain("x01xxxxx").is_fixed());
  ASSERT_FALSE(BitVectorDomain("xxxxxxxx").is_fixed());
  ASSERT_TRUE(BitVectorDomain(BitVector(4, "0000")).is_fixed());
  ASSERT_TRUE(BitVectorDomain(BitVector(8, "1010")).is_fixed());
  ASSERT_TRUE(BitVectorDomain(4, 0).is_fixed());
  ASSERT_TRUE(BitVectorDomain(8, 5).is_fixed());
  ASSERT_TRUE(BitVectorDomain(BitVectorDomain(8, 5)).is_fixed());
  ASSERT_FALSE(
      BitVectorDomain(BitVector(4, "1000"), BitVector(4, "0111")).is_fixed());
  ASSERT_FALSE(
      BitVectorDomain(BitVector(4, "0100"), BitVector(4, "1011")).is_fixed());
  ASSERT_FALSE(
      BitVectorDomain(BitVector(4, "0110"), BitVector(4, "1001")).is_fixed());
  ASSERT_FALSE(
      BitVectorDomain(BitVector(4, "1001"), BitVector(4, "0110")).is_fixed());
}

TEST_F(TestBitVectorDomain, has_fixed_bits)
{
  ASSERT_FALSE(BitVectorDomain(1).has_fixed_bits());
  ASSERT_FALSE(BitVectorDomain(10).has_fixed_bits());
  ASSERT_TRUE(
      BitVectorDomain(BitVector(10, 4), BitVector(10, 5)).has_fixed_bits());
  ASSERT_TRUE(BitVectorDomain("00000000").has_fixed_bits());
  ASSERT_TRUE(BitVectorDomain("11111111").has_fixed_bits());
  ASSERT_TRUE(BitVectorDomain("10110100").has_fixed_bits());
  ASSERT_TRUE(BitVectorDomain("x01xxxxx").has_fixed_bits());
  ASSERT_FALSE(BitVectorDomain("xxxxxxxx").has_fixed_bits());
  ASSERT_TRUE(BitVectorDomain(BitVector(4, "0000")).has_fixed_bits());
  ASSERT_TRUE(BitVectorDomain(BitVector(8, "1010")).has_fixed_bits());
  ASSERT_TRUE(BitVectorDomain(4, 0).has_fixed_bits());
  ASSERT_TRUE(BitVectorDomain(8, 5).has_fixed_bits());
  ASSERT_TRUE(BitVectorDomain(BitVectorDomain(8, 5)).has_fixed_bits());
  ASSERT_FALSE(BitVectorDomain(BitVector(4, "1000"), BitVector(4, "0111"))
                   .has_fixed_bits());
  ASSERT_FALSE(BitVectorDomain(BitVector(4, "0100"), BitVector(4, "1011"))
                   .has_fixed_bits());
  ASSERT_FALSE(BitVectorDomain(BitVector(4, "0110"), BitVector(4, "1001"))
                   .has_fixed_bits());
  ASSERT_FALSE(BitVectorDomain(BitVector(4, "1001"), BitVector(4, "0110"))
                   .has_fixed_bits());
}

TEST_F(TestBitVectorDomain, is_fixed_bit)
{
  BitVectorDomain d(BitVector(4, "1000"), BitVector(4, "1111"));
  ASSERT_TRUE(d.is_fixed_bit(3));
  for (uint32_t i = 0; i < 3; ++i)
  {
    ASSERT_FALSE(d.is_fixed_bit(i));
  }
  ASSERT_DEATH(d.is_fixed_bit(5), "< get_size");
}

TEST_F(TestBitVectorDomain, is_fixed_bit_true)
{
  BitVectorDomain d(BitVector(4, "1000"), BitVector(4, "1110"));
  ASSERT_TRUE(d.is_fixed_bit_true(3));
  for (uint32_t i = 0; i < 3; ++i)
  {
    ASSERT_FALSE(d.is_fixed_bit_true(i));
  }
  ASSERT_DEATH(d.is_fixed_bit(5), "< get_size");
}

TEST_F(TestBitVectorDomain, is_fixed_bit_false)
{
  BitVectorDomain d(BitVector(4, "1000"), BitVector(4, "1110"));
  ASSERT_TRUE(d.is_fixed_bit_false(0));
  for (uint32_t i = 1; i < 4; ++i)
  {
    ASSERT_FALSE(d.is_fixed_bit_false(i));
  }
  ASSERT_DEATH(d.is_fixed_bit(5), "< get_size");
}

TEST_F(TestBitVectorDomain, fix_bit)
{
  BitVectorDomain d("xxxx");
  ASSERT_NO_FATAL_FAILURE(d.fix_bit(0, true));
  ASSERT_EQ(d, BitVectorDomain("xxx1"));
  ASSERT_NO_FATAL_FAILURE(d.fix_bit(0, false));
  ASSERT_EQ(d, BitVectorDomain("xxx0"));
  ASSERT_NO_FATAL_FAILURE(d.fix_bit(1, true));
  ASSERT_EQ(d, BitVectorDomain("xx10"));
  ASSERT_NO_FATAL_FAILURE(d.fix_bit(1, false));
  ASSERT_EQ(d, BitVectorDomain("xx00"));
  ASSERT_NO_FATAL_FAILURE(d.fix_bit(2, true));
  ASSERT_EQ(d, BitVectorDomain("x100"));
  ASSERT_NO_FATAL_FAILURE(d.fix_bit(2, false));
  ASSERT_EQ(d, BitVectorDomain("x000"));
  ASSERT_NO_FATAL_FAILURE(d.fix_bit(3, true));
  ASSERT_EQ(d, BitVectorDomain("1000"));
  ASSERT_NO_FATAL_FAILURE(d.fix_bit(3, false));
  ASSERT_EQ(d, BitVectorDomain("0000"));
  ASSERT_DEATH(d.fix_bit(5, true), "< get_size");
}

TEST_F(TestBitVectorDomain, match_fixed_bits)
{
  for (uint32_t i = 0; i < 3; ++i)
  {
    for (uint32_t j = 0; j < 3; ++j)
    {
      for (uint32_t k = 0; k < 3; ++k)
      {
        std::stringstream ss;
        ss << (i == 0 ? '0' : (i == 1 ? '1' : 'x'))
           << (j == 0 ? '0' : (j == 1 ? '1' : 'x'))
           << (k == 0 ? '0' : (k == 1 ? '1' : 'x'));
        test_match_fixed_bits(ss.str());
      }
    }
  }
}

TEST_F(TestBitVectorDomain, eq)
{
  BitVectorDomain d1("xxxx");
  BitVectorDomain d2("x10x");
  BitVectorDomain d3("x10x");
  ASSERT_TRUE(d2 == d3);
  ASSERT_FALSE(d1 == d2);
  ASSERT_FALSE(d1 == d3);
}

}  // namespace bzlals