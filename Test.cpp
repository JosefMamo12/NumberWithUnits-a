#include "doctest.h"
#include "NumberWithUnits.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

using namespace doctest;
using namespace std;
using namespace ariel;


/*
SUCCED TO READ units.txt
*/
TEST_CASE("read_units function")
{
    ifstream units_file{"units.txt"};
    CHECK_NOTHROW(NumberWithUnits::read_units(units_file));
    ifstream units_file1{"josephUnits.txt"};
    /**
 * units.txt
 **/
    CHECK_NOTHROW(NumberWithUnits::read_units(units_file1));
    CHECK_NOTHROW(NumberWithUnits(5, "cm"));
    CHECK_NOTHROW(NumberWithUnits(5, "m"));
    CHECK_NOTHROW(NumberWithUnits(5, "kg"));
    CHECK_NOTHROW(NumberWithUnits(5, "g"));
    CHECK_NOTHROW(NumberWithUnits(5, "ton"));
    CHECK_NOTHROW(NumberWithUnits(5, "min"));
    CHECK_NOTHROW(NumberWithUnits(5, "sec"));
    CHECK_NOTHROW(NumberWithUnits(5, "ILS"));
    CHECK_NOTHROW(NumberWithUnits(5, "USD"));
    CHECK_NOTHROW(NumberWithUnits(5, "m"));
    /**
 * jsephUnits.txt
 **/
    CHECK_NOTHROW(NumberWithUnits(5, "BTC"));
    CHECK_NOTHROW(NumberWithUnits(5, "hp"));
    CHECK_NOTHROW(NumberWithUnits(5, "watt"));
    CHECK_NOTHROW(NumberWithUnits(5, "lbs"));
    CHECK_NOTHROW(NumberWithUnits(5, "kb"));
    CHECK_NOTHROW(NumberWithUnits(5, "gb"));
    CHECK_NOTHROW(NumberWithUnits(5, "mega"));
    CHECK_NOTHROW(NumberWithUnits(5, "tb"));

    /**
     * Not contains to the text files
     **/
    CHECK_THROWS(NumberWithUnits(5, "THIS"));
    CHECK_THROWS(NumberWithUnits(5, "IS"));
    CHECK_THROWS(NumberWithUnits(5, "BAD"));
    CHECK_THROWS(NumberWithUnits(5, "UNITS"));
}
TEST_CASE("Overloading Operators - Arithmetic cases")
{

    /*
    Should be doing nothing leave the number as it comes
    */
    SUBCASE("=== Unary plus  '+' ===")
    {
        NumberWithUnits a{2, "kg"};
        CHECK(+a == a);
        NumberWithUnits b{-2, "kg"};
        CHECK(+b == b);
        CHECK_FALSE(a == b); //checking again if somthing has been changed
    }
    /*
    Should change the sign of the number
    */

    SUBCASE("=== Unary Minus '-' ===")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{-1000, "g"};
        NumberWithUnits test2{0.231231, "mm"};
        CHECK_EQ(-test0, test1);                    // check if one minus work
        CHECK_EQ(-(-test0), test0);                 // check if double minus works
        CHECK_EQ(-(-(-test0)), test1);              // check if triple minus works
        CHECK_FALSE(-test0 == -test1);              // Now test0 is -1kg and test1 suppose to be 1kg
        CHECK_FALSE(-test2 == test2);               // -0.231231 != 0.231231 checking for num
        NumberWithUnits test3 = (test0 + (-test1)); // test3 = 1kg + (-(1000g)) = 2kg
        NumberWithUnits test4 = -test3;             //test4 = -2 = -test3
        CHECK_EQ(test4, NumberWithUnits{-2, "kg"});
    }
    SUBCASE("=== Operator '+=' ===")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{-1000, "g"};
        NumberWithUnits test2{0.231231, "mm"};
        NumberWithUnits test3{1, "ton"};
        NumberWithUnits test4{600, "cm"};
        NumberWithUnits test5{0.231231, "cm"};

        CHECK_NOTHROW(test0 += test1);                                 //checking two diffrent types succeed to add
        CHECK_EQ(test0, NumberWithUnits{0, "kg"});                     // 1kg +=-1000g = 0; and checking after adding to teso0 line before
        CHECK_EQ(test4 += test5, NumberWithUnits{600.231231, "cm"});   // add same type
        CHECK_EQ(test2 += test2, NumberWithUnits{0.231231 * 2, "mm"}); //self adding
        CHECK_THROWS(test3 += test5);                                  //cannot convert cm -> ton suppose to throw a massage
    }
    SUBCASE("=== Operator '-=' ===")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{-1000, "g"};
        NumberWithUnits test2{0.231231, "mm"};
        NumberWithUnits test3{1, "ton"};
        NumberWithUnits test4{600, "cm"};
        NumberWithUnits test5{0.231231, "cm"};

        CHECK_NOTHROW(test0 -= test1);                               //checking two diffrent types succeed to sub
        CHECK_EQ(test0, NumberWithUnits{2, "kg"});                   // 1kg -=-1000g = 2kg, and checking that it work on the same test0 line before
        CHECK_EQ(test4 -= test5, NumberWithUnits{599.768769, "cm"}); // sub same type
        CHECK_EQ(test2 -= test2, NumberWithUnits{0, "mm"});          //self subing 0.231231 mm - 0.231231 mm = 0 mm;
        CHECK_THROWS(test3 -= test5);                                //cannot convert cm -> ton supposed to throw a massage
    }
    SUBCASE("=== Operator'++' ===")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{-1000, "g"};
        NumberWithUnits test2{0.231231, "mm"};
        CHECK_EQ(test0++, NumberWithUnits{1, "kg"});                        //  regular positive number
        CHECK_EQ(test1++, NumberWithUnits{-1000, "g"});                     // regualr negative number
        CHECK_EQ(test2++, NumberWithUnits{0.231231, "mm"});                 // number with reminder
        CHECK_EQ(++test0, NumberWithUnits{3, "kg"});                        // changed twich becasuse the prefix in this line and post fix 2 line above
        CHECK_EQ(++test1, NumberWithUnits{-998, "g"});                      //same here
        CHECK_EQ(++test2, NumberWithUnits{2.231231, "mm"});                 // same here
        CHECK_EQ(++++++++++++++++++++test1, NumberWithUnits{-988, "g"});    //10 ++ which means -998 + 10 = -988
        CHECK_EQ(-(++++++++++++++++++++test0), NumberWithUnits{-13, "kg"}); // combine between two aritmetic opertor
    }
    SUBCASE("=== Operator '--' ===")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{-1000, "g"};
        NumberWithUnits test2{0.231231, "mm"};
        CHECK_EQ(test0--, NumberWithUnits{1, "kg"});                       //  regular positive number
        CHECK_EQ(test1--, NumberWithUnits{-1000, "g"});                    // regualr negative number
        CHECK_EQ(test2--, NumberWithUnits{0.231231, "mm"});                // number with reminder
        CHECK_EQ(--test0, NumberWithUnits{-1, "kg"});                      // changed twich becasuse the prefix in this line and post fix 2 line above
        CHECK_EQ(--test1, NumberWithUnits{-1002, "g"});                    //same here
        CHECK_EQ(--test2, NumberWithUnits{-1.768769, "mm"});               // same here
        CHECK_EQ(--------------------test1, NumberWithUnits{-1012, "g"});  //10 -- which means -1002 - 10 = -1012
        CHECK_EQ(-(--------------------test0), NumberWithUnits{11, "kg"}); // combine between two aritmetic opertor
    }
    SUBCASE("=== Operator regular '+' ===")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{-1000, "g"};
        NumberWithUnits test2{0.231231, "mm"};
        NumberWithUnits test3{1, "ton"};
        NumberWithUnits test4{600, "cm"};

        CHECK_EQ((test0 + test1), NumberWithUnits{0, "kg"});                 // convert g -> kg and 1kg + (-1000g)*0.001
        CHECK_EQ((test3 + test0 + test1), NumberWithUnits{1, "ton"});        // two converts to ton
        CHECK_EQ((test1 + test0 + test3), NumberWithUnits{1000000, "g"});    // same as before but now the left side be gram
        CHECK_EQ((test3 + test0 + (-test1)), NumberWithUnits{1.002, "ton"}); // two converts to ton and using minus binary
        CHECK_EQ(test2 + test4, NumberWithUnits{6000.231231, "mm"});
        CHECK_THROWS(test0 + test2);         // add between two diffrent types
        CHECK_THROWS(test0 + test1 + test2); // add between two good type  and the third one is diffrent type
    }
    SUBCASE("=== Operator regular '-' ===")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{-1000, "g"};
        NumberWithUnits test2{0.231231, "mm"};
        NumberWithUnits test3{1, "ton"};
        NumberWithUnits test4{600, "cm"};

        CHECK_EQ((test0 - test1), NumberWithUnits{2, "kg"});                 // convert g -> kg and 1kg - (-1000g)*0.001
        CHECK_EQ((test3 - test0 - test1), NumberWithUnits{1, "ton"});        // two converts to ton
        CHECK_EQ((test1 - test0 - test3), NumberWithUnits{-1002000, "g"});   // same as before but now the left side is g = grams
        CHECK_EQ((test3 - test0 - (-test1)), NumberWithUnits{0.998, "ton"}); // two converts to ton and using minus binary
        CHECK_EQ(test2 - test4, NumberWithUnits{-5999.768769, "mm"});
        CHECK_THROWS(test0 - test2);         // add between two diffrent types
        CHECK_THROWS(test0 - test1 - test2); // add between two good type  and the third one is diffrent type
    }
    SUBCASE("=== Operator multiplay '*' ===")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{-1000, "g"};
        NumberWithUnits test2 = test0 * test1.num(); // assigning

        CHECK_EQ(test2, NumberWithUnits{-1000, "kg"});
        CHECK_EQ(5 * test0, NumberWithUnits{5, "kg"});   // prefix multiplay
        CHECK_EQ(test0 * 5, NumberWithUnits{5, "kg"});   // postfix multiplay
        CHECK_EQ(-5 * test0, NumberWithUnits{-5, "kg"}); // positive mult by negative
        CHECK_EQ(test0 * -5, NumberWithUnits{-5, "kg"}); // same but postfix
        CHECK_EQ(-5 * -test0, NumberWithUnits{5, "kg"}); // negative mult by negative
        CHECK_EQ(-test0 * -5, NumberWithUnits{5, "kg"}); //
    }
}
TEST_CASE("Overloading Operators - Boolean cases")
{
    SUBCASE("=== Operator '==' === ")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{1000, "ILS"};
        CHECK(NumberWithUnits(1, "BTC") == NumberWithUnits(207722.2032, "ILS"));       // BTC from jospehUnits.txt to USD from both ot them to ILS from units.txt
        CHECK_FALSE(NumberWithUnits(1, "BTC") == NumberWithUnits(207722.1012, "ILS")); // not good this one change the tolerance to be smaller
        CHECK(NumberWithUnits(35, "cm") == NumberWithUnits{0.35, "m"});                // cm -> m
        CHECK(NumberWithUnits(3.5, "km") == NumberWithUnits{350000, "cm"});            // km -> cm -> m; 2 checks on convert table
        CHECK(NumberWithUnits(3.5, "km") == NumberWithUnits{3500000, "mm"});           // km -> m -> cm -> mm; 3 checks on convert unit table
        CHECK(NumberWithUnits{10, "ton"} == NumberWithUnits{10000000, "g"});           // 2 convert units that used in 3rd party tong -> kg -> g
        CHECK(NumberWithUnits{2.121311313, "mm"} == NumberWithUnits{2.121311313, "mm"});
        CHECK_FALSE(NumberWithUnits{2, "kg"} == NumberWithUnits{0.2, "ton"});  // should not be true because it 0.002
        CHECK_THROWS(operator==(NumberWithUnits{2, "kg"} , NumberWithUnits{2, "cm"}));     // Units here cannot be converted
        CHECK_FALSE(NumberWithUnits{0.3, "USD"} == NumberWithUnits{2, "ILS"}); // 0.3 USD != 2 ILS
        CHECK_THROWS(operator==(test0, test1));                                // kg -> ILS no such a convert
        CHECK_THROWS(operator==(test1, test0));                                //  ILS -> kg no such a convert
    }
    SUBCASE("=== Operator '!=' === ")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{1000, "g"};
        NumberWithUnits test2{2, "hour"};
        NumberWithUnits test3{0.25, "ton"};
        NumberWithUnits test4{-50, "ILS"};
        NumberWithUnits test5{-16.5, "USD"};

        CHECK(test0 != test3);                                                  // 1 kg != 250 kg
        CHECK_FALSE(test0 != test1);                                            // 1 kg == 1000g
        CHECK_THROWS(operator!=(test2 , test1));                                           // cannot convert g to hours
        CHECK((test1 + test0 + NumberWithUnits{248000, "g"}) != test3 + test0); // cannot convert g to hours ; 250000 g != 0.26 ton
        CHECK(test4 != test5);                                                  // -50 ILS == -16.5 USD ; NEGATIVE CHECK
    }
    SUBCASE("=== Operator '<='  === ")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{1000, "g"};
        NumberWithUnits test2{2, "hour"};
        NumberWithUnits test3{-5, "hour"};
        NumberWithUnits test4{0.25, "ton"};
        NumberWithUnits test5{-50, "ILS"};
        NumberWithUnits test6{-16.5, "USD"};

        CHECK(test0 <= test0);                  // same experssion checking
        CHECK(test0 <= test1);                  // should be good because 1kg == 1000g diffrenent types
        CHECK(test3 <= test2);                  // regular same type -5 hours is less then 2 hour
        CHECK(test3 <= -test2);                 // two negatative experrsions  -5 <= -2
        CHECK_FALSE(test4 <= test1);            //  1kg is not small or equall than 0.25
        CHECK(5 * test5 <= test6);              // -250 ILS IS LESS THAN -16.5 USD
        CHECK_THROWS(operator<=(test2, test4)); // Cannot convert ton to hours
    }
    SUBCASE("=== Operator '<'  === ")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{1000, "g"};
        NumberWithUnits test2{2, "hour"};
        NumberWithUnits test3{-5, "hour"};
        NumberWithUnits test4{0.25, "ton"};
        NumberWithUnits test5{-50, "ILS"};
        NumberWithUnits test6{-16.5, "USD"};

        CHECK_FALSE(test0 < test0);            // same experssion checking
        CHECK_FALSE(test0 < test1);            // should be false because 1kg == 1000g, diffrenent types
        CHECK(test3 < test2);                  // regular same type -5 hours is less then 2 hour
        CHECK(test3 < -test2);                 // two negatative experrsions  -5 < -2
        CHECK_FALSE(test4 < test1);            // 1kg is not less than 0.25ton
        CHECK(5 * test5 < test6 * 2);          // -250 ILS IS LESS THAN -33 USD
        CHECK_THROWS(operator<(test2, test4)); // Cannot convert ton to hours
    }
    SUBCASE("=== Operator '>='  === ")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{1000, "g"};
        NumberWithUnits test2{2, "hour"};
        NumberWithUnits test3{-5, "hour"};
        NumberWithUnits test4{0.25, "ton"};
        NumberWithUnits test5{-50, "ILS"};
        NumberWithUnits test6{-16.5, "USD"};

        CHECK(test1 >= test1);                  // same experssion checking
        CHECK(test1 >= test0);                  // should be good because 1kg == 1000g diffrenent types
        CHECK(test2 >= test3);                  // regular same type -5 hours is less then 2 hour
        CHECK(-test3 >= test2);                 // two negatative experrsions  -2 >= -5
        CHECK(test4 >= test1);                  //  0.25 ton is bigger or equall to 1000 g
        CHECK(test5 >= 5 * test6);              // -82.5 USD IS LESS THAN -50 ILS
        CHECK_THROWS(operator>=(test2, test4)); // Cannot convert ton to hours
    }
    SUBCASE("=== Operator '>'  === ")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{1000, "g"};
        NumberWithUnits test2{2, "hour"};
        NumberWithUnits test3{-5, "hour"};
        NumberWithUnits test4{0.25, "ton"};
        NumberWithUnits test5{-50, "ILS"};
        NumberWithUnits test6{-16.5, "USD"};

        CHECK_FALSE(test1 > test1);            // same experssion checking
        CHECK_FALSE(test1 > test0);            // should be false because 1kg == 1000g, diffrenent types
        CHECK(test2 > test3);                  // regular same type -5 hours is less then 2 hour
        CHECK(-test2 > test3);                 // two negatative experrsions  -2 > -5
        CHECK_FALSE(test1 > test4);            // 1kg is not less than 0.25ton
        CHECK(test5 * 2 > 5 * test6);          // -100 ILS IS bigger than 82.5 USD
        CHECK_THROWS(operator>(test4, test2)); // Cannot convert ton to hours
    }
}
TEST_CASE("=== IO Operators ===")
{
    SUBCASE("=== Input operator '<<' ===")
    {
        NumberWithUnits a;
        istringstream sample_input{"700 [kg]"};
        sample_input >> a;
        CHECK_EQ(a, NumberWithUnits{700, "kg"});
        NumberWithUnits b;
        istringstream sample_input1{"0.5 [             cm      ]"}; // with alot of space
        sample_input1 >> b;
        CHECK_EQ(b, NumberWithUnits{0.5, "cm"});
        NumberWithUnits c;
        istringstream sample_input2{"-30[ ILS      ]"}; // negative number
        sample_input2 >> c;
        CHECK_EQ(c, NumberWithUnits{-30, "ILS"});
        NumberWithUnits d;
        istringstream sample_input3{"0.22222232312123 [ kg ]"};
        sample_input3 >> d;
        CHECK_EQ(d, NumberWithUnits{0.22222232312123, "kg"});
        NumberWithUnits e;
        istringstream sample_input4{"dsldkk-30 [ ILS     ]"};
        CHECK_THROWS(sample_input4 >> e); // Bad unit type thats not associate with the convert table
        NumberWithUnits f;
        istringstream sample_input5{"[0.22222232312123  kg      ]"};
        CHECK_THROWS(sample_input5 >> f); // bad format
    }
    SUBCASE("=== output operator '>>' ===")
    {
        NumberWithUnits test0{1, "kg"};
        NumberWithUnits test1{0.251231, "g"};
        NumberWithUnits test2{2, "hour"};
        NumberWithUnits test3{-255, "BTC"};
        ostringstream test0_s;
        test0_s << test0;
        CHECK_EQ(test0_s.str(), string("1[kg]"));
        ostringstream test1_s;
        test1_s << test1;
        CHECK_EQ(test1_s.str(), string("0.251231[g]"));
        ostringstream test2_s;
        test2_s << test2;
        CHECK_EQ(test2_s.str(), string("2[hour]"));
        ostringstream test3_s;
        test3_s << test3;
        CHECK_EQ(test3_s.str(), string("-255[BTC]"));
    }
}