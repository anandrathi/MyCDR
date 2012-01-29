#pragma once
class UnitTest
{
public:
	UnitTest(void);
	~UnitTest(void);
	void * _context;
	bool TestScriptConstantVar(void);
	bool TestCharIterator(void);
	bool TestFixIterator(void);
	bool TestCharIteratorN(void);
	bool TestFixIteratorN(void);


	bool TestMMapFileReader(void);
	bool TestCharRecordParse(void);
	bool TestCharRecord(void);
	bool TestCharFileRead(void);
	bool TestChar(void);
	bool TestFixedRecordCOntainer(void);
	bool TestCharRecordCOntainer(void);
	bool TestDirMoniter(void);
	bool TestRecordIterator(void);
	bool TestTCLBind(void);
	bool TestFILESCANTIMEOUT(void);
};

