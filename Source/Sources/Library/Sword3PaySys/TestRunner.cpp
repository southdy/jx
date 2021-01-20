/**************************************************/
/*                                                */
/*  �ļ���:    TestRunner.cpp                     */
/*  ����    :  ά��ÿ�����Ե㣬��������ִ��       */
/*             ��Ӧ��Test����                     */
/*                                                */
/*	����	 : Liu Wansong                        */
/*	�������� : 8/22/2002                          */
/*  �޸����� : 8/26/2002                          */
/**************************************************/

#include "TestCase.h"
#include "TestSuite.h"
#include "TestCaller.h"
#include "TestRunner.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void TestRunner::printBanner ()
{
    cout << "Usage: driver [ -wait ] testName, where name is the name of a test case class" << endl;
}


void TestRunner::run (string testCase)
{
    if (testCase == "")
	{
        printBanner ();
        return;
    }

    Test *testToRun = NULL;

    for (mappings::iterator it = m_mappings.begin ();
            it != m_mappings.end ();
            ++it) {
        if ((*it).first == testCase) {
            testToRun = (*it).second;
            run (testToRun);
        }
    }

    if (!testToRun) {
        cout << "Test " << testCase << " not found." << endl;
        return;
    } 
}


TestRunner::~TestRunner ()
{
    for (mappings::iterator it = m_mappings.begin ();
             it != m_mappings.end ();
             ++it)
        delete it->second;

}


void TestRunner::run (Test *test)
{
    TextTestResult  result;

    test->run (&result);

    cout << result << endl;
}

