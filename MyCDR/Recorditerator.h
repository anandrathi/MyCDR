#pragma once

#include "CDRTypes.h"
#include "Record.h"
class FileReader;
class CharRecordIteraor;
class Recorditerator : public MyCDRBASE
{
public:
	enum RECORDPOSITION { RECORDPOSITION_BEGIN=-1, RECORDPOSITION_INVALID=-2, RECORDPOSITION_END =-3 , RECORDPOSITION_OK =0 };
	enum  RECORDERROR { RECORDERROR_BEGIN, RECORDERROR_GOOD,  RECORDERROR_BADLENGTH , RECORDERROR_INCOMPLETE} ;
protected:

	friend class CharRecordIteraor;
	friend class FileReader;
	Recorditerator(RecordDetails*	pRecordDetails):_RecordDetails(pRecordDetails)
	{
		_RecordNumber=0;
		_TotalRecords=0;
	}
	~Recorditerator(void);
	char * _currentRowData;
	char * _currentColData;
	char * _data;
	RECORDPOSITION _pos;
	RECORDERROR _error;
	unsigned long   _filesize;
	RecordDetails* _RecordDetails;
public:
	virtual void begin(void)=0;
	inline RECORDPOSITION now(void) {return _pos;}
	inline RECORDERROR getState(void) {return  _error; }; 
	unsigned long   _RecordNumber;
	unsigned long   _TotalRecords;
	inline void setData(char * data){_data=data; 	_currentRowData=_data; }
	inline char * getData(void){return _data ;	}
	inline void setfilesize(unsigned long   filesize){_filesize=filesize ; }
	inline unsigned long  getfilesize(void){return _filesize; }
	inline 
	virtual AR_INT64 Prev(void)=0;
	virtual AR_INT64 Next(void)=0;
	virtual AR_INT64 getPos(void)=0;
	virtual bool isEnd(void )=0;
	virtual void goBegin(void )=0;
	virtual void goEnd(void )=0;
	virtual AR_INT64 goTo(AR_INT64 index)=0;
	void dump(void);
	virtual void setScriptVar(void *)=0;
	virtual void InitScriptVar(void *p, void *s)=0;

//	virtual void setTCLVar(void *)=0;
	virtual void setSTRVar(std::map<std::string, std::string> &s)=0;
	//virtual void setSTRVar(void * )=0;
	virtual RecordDetails*	getRecordDetails(void){return _RecordDetails;};
//	void SetRecordDetails(RecordDetails* pRecordDetails ) {_RecordDetails = pRecordDetails;}


};

