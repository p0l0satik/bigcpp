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
TEST_F(FactoryTests, pol_) {
    auto s = fact.CreateFunction("exp", {2});
    ASSERT_EQ((*s)(2), 4);
    ASSERT_EQ((*s)(3), 8);
    ASSERT_EQ(s->GetDeriv(2), log(2)*pow(2, 2));
    ASSERT_EQ(s->ToString(), "2.000000^x");
}


int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}