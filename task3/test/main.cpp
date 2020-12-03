#include <gtest/gtest.h>
#include "../bib/functions.hpp"
#include <iostream>

class FactoryTests : public ::testing::Test {
public:
    FunFactory fact;
    FactoryTests() { /* init protected members here */ }
    ~FactoryTests() { /* free protected members here */ }
    void SetUp() { }
    void TearDown() { /* called after every test */ }

protected:
    /* none yet */
};

// CONSTANT
TEST_F(FactoryTests, const_0) {
    
    auto c = fact.CreateFunction("constant",{0});

    ASSERT_EQ((*c)(100), 0);
    ASSERT_EQ((*c)(0), 0);
    ASSERT_EQ(c->ToString(), "0.000000");
    ASSERT_EQ(c->GetDeriv(0), 0);
    ASSERT_EQ(c->GetDeriv(0), 0);
}
// IDENTICAL
TEST_F(FactoryTests, ident_simple) {
    
    auto i = fact.CreateFunction("identical", {1, 0});

    ASSERT_EQ((*i)(0), 0);
    ASSERT_EQ((*i)(10), 10);
    ASSERT_EQ(i->GetDeriv(0), 1);
    ASSERT_EQ(i->GetDeriv(10), 1);
    ASSERT_EQ(i->ToString(), "1.000000*x");
}

TEST_F(FactoryTests, ident_with_coefs) {
    
    auto i = fact.CreateFunction("identical", {2, 3});

    ASSERT_EQ((*i)(0), 3);
    ASSERT_EQ((*i)(10), 23);
    ASSERT_EQ(i->GetDeriv(0), 2);
    ASSERT_EQ(i->GetDeriv(10), 2);
    ASSERT_EQ(i->ToString(), "2.000000*x+3.000000");
}


// Sedate
TEST_F(FactoryTests, sedate_0) {
    auto s = fact.CreateFunction("sedate", {0});
    ASSERT_EQ((*s)(10), 1);
    ASSERT_EQ(s->GetDeriv(10), 0);
    ASSERT_EQ((*s)(0), 1);
    ASSERT_EQ(s->ToString(), "x^0.000000");
}

TEST_F(FactoryTests, sedate_10) {
    auto s = fact.CreateFunction("sedate", {10});
    ASSERT_EQ((*s)(0), 0);
    ASSERT_EQ((*s)(2), 1024);
    ASSERT_EQ(s->GetDeriv(2), 512 * 10);
    ASSERT_EQ(s->ToString(), "x^10.000000");
}

TEST_F(FactoryTests, sedate_1) {
    auto s = fact.CreateFunction("sedate", {1});
    ASSERT_EQ((*s)(0), 0);
    ASSERT_EQ((*s)(2), 2);
    ASSERT_EQ(s->GetDeriv(2), log(2));
    ASSERT_EQ(s->ToString(), "x^1.000000");
}

TEST_F(FactoryTests, sedate_neg1) {
    auto s = fact.CreateFunction("sedate", {-1});
    ASSERT_EQ((*s)(2), 0.5);
    ASSERT_EQ(s->GetDeriv(2), -1.0 / 4);
    ASSERT_EQ(s->ToString(), "x^-1.000000");
}

// EXPONENT
TEST_F(FactoryTests, exp_neg2) {
    auto s = fact.CreateFunction("exp", {-2});
    ASSERT_EQ((*s)(2), 4);
    ASSERT_EQ((*s)(3), -8);
    // ASSERT_EQ(s->GetDeriv(2), log(-2)*pow(-2, 2)); is complex
    ASSERT_EQ(s->ToString(), "-2.000000^x");
}

TEST_F(FactoryTests, exp_2) {
    auto s = fact.CreateFunction("exp", {2});
    ASSERT_EQ((*s)(2), 4);
    ASSERT_EQ((*s)(3), 8);
    ASSERT_EQ(s->GetDeriv(2), log(2)*pow(2, 2));
    ASSERT_EQ(s->ToString(), "2.000000^x");
}

// POLYNOMIAL
TEST_F(FactoryTests, pol_15x3_3x2_7) {
    auto p = fact.CreateFunction("polynomial", {7, 0, 3, 15});
    ASSERT_EQ((*p)(1), 15+3+7);
    ASSERT_EQ((*p)(3), 15*27 + 3*9 + 7);
    ASSERT_EQ((*p)(0), 7);
    ASSERT_EQ(p->GetDeriv(2), 45*4 + 6*2);
    ASSERT_EQ(p->GetDeriv(1), 45*1 + 6*1);
    ASSERT_EQ(p->GetDeriv(0), 0);

    ASSERT_EQ(p->ToString(), "7 + 3*x^2 + 15*x^3");
}

// ARITHMETICS
TEST_F(FactoryTests, arithmetics) {
    auto c0 = fact.CreateFunction("constant", {0});
    auto c1 = fact.CreateFunction("constant", { 1.0});
    auto i3= fact.CreateFunction("identical", { 3.0, -1.0});
    auto i1= fact.CreateFunction("identical", { 3.0, -0});
    auto s3 = fact.CreateFunction("sedate", { 2.0});
    auto e10 = fact.CreateFunction("exp", {10});
    auto p = fact.CreateFunction("polynomial", {1, -2, 3});
    auto p2 = fact.CreateFunction("polynomial", {7, 0, 3, 15});

    auto sum1 = c0 + c1;
    ASSERT_EQ((*sum1)(10), 1);
    auto bigsum = c0 + c1 + c1 + c1 + c1;
    ASSERT_EQ((*bigsum)(10), 4);

    auto c1_i3 = c1 + i3;
    ASSERT_EQ((*c1_i3)(10), 3 * 10 - 1 + 1);
    auto c1_s3 = c1 + s3;
    ASSERT_EQ((*c1_s3)(10), 1 + 100);
    auto c1_e10 = c1 + e10;
    ASSERT_EQ((*c1_e10)(2), 100 + 1);
    auto c1_p = c1 + p;
    ASSERT_EQ((*c1_p)(10), 3* 10 * 10 - 20 + 1 + 1);


    auto sub = s3 - i1;
    ASSERT_EQ((*sub)(10), 100 - 30);
    auto mul = p * e10;
    ASSERT_EQ((*mul)(2), (3 * 4 - 4 + 1) * 100);
    auto mul_p = p2 * p;
    ASSERT_EQ((*mul_p)(10), (3 * 100 - 2 * 10 + 1) * (15000 + 300 + 7));

    auto divp2_p = p2 / p;
    ASSERT_EQ((*divp2_p)(10), (15000.0 + 300 + 7) / (3 * 100 - 2 * 10 + 1));
    auto divp_p2 = p / p2;
    ASSERT_EQ((*divp_p2)(10), (3 * 100.0 - 2 * 10 + 1) / (15000 + 300 + 7));

    auto ultimate = (c0 + c1) * (c1 + s3) / ((s3 - p2) / p) - s3 + i1;
    ASSERT_EQ((*ultimate)(10), 101.0 / ((100.0 - (15000 + 300 + 7)) / (3 * 100 - 2 * 10 + 1)) - 100 + 30);
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}