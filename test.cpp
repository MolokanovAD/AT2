#include "pch.h"
#include "../ProjectAT2WithTest/SyntaxTree.h"
#include "../ProjectAT2WithTest/BinaryOperator.h"
#include "../ProjectAT2WithTest/Brackets.h"
#include "../ProjectAT2WithTest/Concat.h"
#include "../ProjectAT2WithTest/DFA.h"
#include "../ProjectAT2WithTest/Functions.h"
#include "../ProjectAT2WithTest/Group.h"
#include "../ProjectAT2WithTest/LastNode.h"
#include "../ProjectAT2WithTest/Leaf.h"
#include "../ProjectAT2WithTest/Node.h"
#include "../ProjectAT2WithTest/Operator.h"
#include "../ProjectAT2WithTest/Or.h"
#include "../ProjectAT2WithTest/Positive.h"
#include "../ProjectAT2WithTest/Regexpr.h"
#include "../ProjectAT2WithTest/Repeat.h"
#include "../ProjectAT2WithTest/Screen.h"
#include "../ProjectAT2WithTest/State.h"
#include "../ProjectAT2WithTest/UnaryOperator.h"
#include "../ProjectAT2WithTest/ZeroOperator.h"
using namespace regexpr;

TEST(StringCheck, WrongStringTest) {
	SyntaxTree a;
	ASSERT_THROW(a.create("abc++f"), std::exception);
	ASSERT_THROW(a.create("abc{3,5}+a"), std::exception);
	ASSERT_THROW(a.create("{0}af"), std::exception);
	ASSERT_THROW(a.create("(abc))"), std::exception);
	ASSERT_THROW(a.create("(a(b)"), std::exception);
	ASSERT_THROW(a.create("a{3,4"), std::exception);
	ASSERT_THROW(a.create("aeg|.wd"), std::exception);
}
TEST(OperatorCheck, RepeatTest) {
	ASSERT_TRUE(match("abc{3,5}eg", "abccceg"));
	ASSERT_TRUE(match("abc{3,5}eg", "abcccceg"));
	ASSERT_TRUE(match("abc{3,5}eg","abccccceg" ));
	ASSERT_FALSE(match("abc{3,5}eg", "abcccccceg"));
	ASSERT_TRUE(match("a{0}", ""));
	ASSERT_TRUE(match("a{0}", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
	ASSERT_TRUE(match("a(b|cek|d){3,5}", "acekbbdb"));
	ASSERT_TRUE(match("a(b|cek|d){3,5}", "acekcekcekcekcek"));
	ASSERT_THROW(match("ab{3,}", "abbbb"),std::exception);
	ASSERT_THROW(match("ab{3,2}", "abbbb"), std::exception);
	ASSERT_NO_THROW(match("ab{3,3}", "abbbb"));
	
}
TEST(OperatorCheck, PositiveTest) {
	ASSERT_TRUE(match("abc+eg", "abccceg"));
	ASSERT_TRUE(match("ab(c|de)+eg", "abcccceg"));
	ASSERT_TRUE(match("ab(c|de)+eg", "abccccdededeeg"));
	ASSERT_FALSE(match("ab(c|de)+eg", "abccccdedeg"));
	ASSERT_TRUE(match("a+", "a"));
	ASSERT_FALSE(match("a+", ""));
	ASSERT_TRUE(match("a+", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"));
}
TEST(OperatorCheck, ConcatAndOrTest) {
	ASSERT_TRUE(match("abceg", "abceg"));
	ASSERT_TRUE(match("ab(c|de)eg", "abceg"));
	ASSERT_TRUE(match("ab(c|de)eg", "abdeeg"));
	ASSERT_TRUE(match("ab((c|de)|e)g", "abcg"));
	ASSERT_TRUE(match("ab((c|d.e)|e)g", "abdeg"));
	ASSERT_TRUE(match("ab((c|de)|e)g", "abeg"));
}
TEST(OperatorCheck, GroupTest) {
	ASSERT_TRUE(match("(3:abceg)", "abceg"));
	ASSERT_TRUE(match("(5:ab(c|de)eg)", "abceg"));
	ASSERT_TRUE(match("(5:ab(c|de)eg)", "abdeeg"));
	ASSERT_TRUE(match("(5:ab(c|de)eg)", "abceg"));
	ASSERT_TRUE(match("(5:ab(c|de)eg)", "abdeeg"));
	ASSERT_TRUE(match("(5:ab(c|de|^)eg)", "abeg"));
}
TEST(LibFunctionsCheck, IntersectionTest) {
	SyntaxTree first("a{0}b+c{1,2}d");
	SyntaxTree second("abcd+");
	DFA f(first.firstPositions(), first.buildFPTable(), first.getExpression(), first.getAlphabet());
	DFA s(second.firstPositions(), second.buildFPTable(), second.getExpression(), second.getAlphabet());
	ASSERT_TRUE(match(intersection("a{0}b+c{1,2}d", "abcd+"), "abcd"));
	ASSERT_TRUE(intersection(f, s).match("abcd"));
	ASSERT_FALSE(match(intersection("a{0}b+c{1,2}d", "abcd+"), "aabcd"));
	ASSERT_FALSE(intersection(f, s).match("aabcd"));
	ASSERT_FALSE(match(intersection("a{0}b+c{1,2}d", "abcd+"), "abbbcd"));
	ASSERT_FALSE(intersection(f, s).match("abbbcd"));
	ASSERT_FALSE(match(intersection("a{0}b+c{1,2}d", "abcd+"), "abcdddd"));
	ASSERT_FALSE(intersection(f, s).match("abcdddd"));
}
TEST(LibFunctionsCheck, AdditionTest) {
	SyntaxTree a("ab+");
	DFA ad(a.firstPositions(), a.buildFPTable(), a.getExpression(), a.getAlphabet());
	DFA add = addition(ad);
	add.minimize();
	ASSERT_TRUE(add.match("abaabbbbababbaabb"));
	ASSERT_FALSE(add.match("abbbbbbbbbbbbbbbbbbbbbb"));
	ASSERT_TRUE(add.match("bbbbbbbbbbbbbbb"));
	ASSERT_TRUE(add.match("aaaaaaaaaaaaaaaaa"));
	a.create("(abc)+");
	DFA ad2(a.firstPositions(), a.buildFPTable(), a.getExpression(), a.getAlphabet());
	DFA add2 = addition(ad2);
	add2.minimize();
	ASSERT_TRUE(add2.match("abcc"));
	ASSERT_FALSE(add2.match("abcabcabcabc"));
	ASSERT_FALSE(add2.match("abc"));
	ASSERT_TRUE(add2.match("cccccc"));
	ASSERT_TRUE(add2.match(""));
}
TEST(LibFunctionsCheck, KPAthTest) {
	SyntaxTree a("ab+c{0}d");
	DFA dfa(a.firstPositions(), a.buildFPTable(), a.getExpression(), a.getAlphabet());
	dfa.minimize();
	SyntaxTree b(recoverRegularExpression(dfa));
	DFA dfa2(b.firstPositions(), b.buildFPTable(), b.getExpression(), b.getAlphabet());
	dfa2.minimize();
	std::string s1 = "abbbbbbbbbd", s2 = "abcd", s3 = "accccccccd", s4 = "abbbbbbbccccccccccccccccccccccd", s5 = "aabcd", s6 = "bd";
	ASSERT_EQ(dfa.match(s1), dfa2.match(s1));
	ASSERT_EQ(dfa.match(s2), dfa2.match(s2));
	ASSERT_EQ(dfa.match(s3), dfa2.match(s3));
	ASSERT_EQ(dfa.match(s4), dfa2.match(s4));
	ASSERT_EQ(dfa.match(s5), dfa2.match(s5));
	ASSERT_EQ(dfa.match(s6), dfa2.match(s6));
}