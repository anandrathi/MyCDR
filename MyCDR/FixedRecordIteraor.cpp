#include "FixedRecordIteraor.h"
#include "ace/Log_Msg.h"
#include "cpptcl.h"
#include "FixedLocationRecord.h"
#include <sqrat.h>

FixedRecordIteraor::FixedRecordIteraor(RecordDetails* pRecordDetails):Recorditerator(pRecordDetails)
{
	ACE_TRACE("FixedRecordIteraor::FixedRecordIteraor");
	_data=0;
	_RecordNumber=0;
	_TotalRecords=0;
	_currentRowData=_data;
	_currentColData=_currentRowData;
	_reclen=0;
	RecordDetails::FIELDDETAILS::iterator it = _RecordDetails->GetFieldDetails()->begin();
	RecordDetails::FIELDDETAILS::iterator eit = _RecordDetails->GetFieldDetails()->end();
	for(;it !=eit ; it++ )
	{
		int len = (it->END - it->START) + 1;
		_reclen+=len;
	}

}


FixedRecordIteraor::~FixedRecordIteraor(void)
{
}


AR_INT64 FixedRecordIteraor::Prev(void)
{
	return 0;
}

void FixedRecordIteraor::begin(void)
{
	_currentRowData=_data;
	_currentColData=_currentRowData;
	_pos	= Recorditerator::RECORDPOSITION_BEGIN;
	_error	= Recorditerator::RECORDERROR_BEGIN;
}

AR_INT64 FixedRecordIteraor::Next(void)
{
	ACE_TRACE("FixedRecordIteraor::Next");
	PARSESTATE s;
	char * ps=_currentRowData;
	char * psCol=_currentRowData;
	char * pe=_currentRowData;
	char * end = _data + this->_filesize;
	RecordDetails::FIELDDETAILS::iterator it = _RecordDetails->GetFieldDetails()->begin();
	RecordDetails::FIELDDETAILS::iterator eit = _RecordDetails->GetFieldDetails()->end();
	int LINE_SEPERATOR=_RecordDetails->LINE_SEPERATOR;

	psCol=ps;
	if ( _currentRowData >= end )	
	{
		_pos = Recorditerator::RECORDPOSITION_END;
		return Recorditerator::RECORDPOSITION_END;
	}
	int i =0;
	while(1) 
	{
		if(*pe==LINE_SEPERATOR)
		{
			break; 
		}
		if(pe>=end)
		{
			break;
		}
		pe++;
		i++;
	}
	if(pe<=end)
	{
		if(pe<end)
		{
			_currentRowData=pe+1;
		}
		if(pe==end)
		{
			_currentRowData=pe;
		}
	}
	_pos = Recorditerator::RECORDPOSITION_OK;

	if( (pe-ps) != this->_reclen)
	{
		//bad record
		ACE_ERROR((LM_ERROR, "(%t) FixedRecordIteraor::Next  Length=%d dosent Match Data Len=%d\n",  this->_reclen ,  (pe-ps) ));
		_error=Recorditerator::RECORDERROR_BADLENGTH;
	}
	else 
	{
		_error=RECORDERROR::RECORDERROR_GOOD;
		for(;it!= eit ;it++)
		{
			it->END-it->START;
			if(it->_RecordData.cell.capacity() < it->_RecordData.len)
			{
				it->_RecordData.cell.resize(it->_RecordData.len+1+1, 0);
			}
			memcpy(&it->_RecordData.cell[0], psCol, it->_RecordData.len);
			psCol+=it->_RecordData.len;
		}
	}
	//if ( _currentRowData >= end )	
	//{
	//	_pos = Recorditerator::RECORDPOSITION_END;
	//}

	_RecordNumber++;
	 return _RecordNumber;
}

AR_INT64 FixedRecordIteraor::getPos(void)
{
	return 0;
}

bool FixedRecordIteraor::isEnd(void )
{
	return true;
}

void FixedRecordIteraor::goBegin(void )
{
	return ;
}

void FixedRecordIteraor::goEnd(void )
{
	return ;
}

AR_INT64 FixedRecordIteraor::goTo(AR_INT64 index)
{
	return 0;
}

void FixedRecordIteraor::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%D)(%t) FixedRecordIteraor \n"));
}

void FixedRecordIteraor::setSTRVar(std::map<std::string, std::string> &s)
{
	ACE_TRACE("(%D)(%t) FixedRecordIteraor::setSTRVar");
	RecordDetails::FIELDDETAILS::iterator it =  _RecordDetails->GetFieldDetails()->begin();
	for(;it!=_RecordDetails->GetFieldDetails()->end();it++)
	{
		char *addr =  const_cast<char*>(it->_RecordData.cell.data()); 
		s[it->NAME]=addr ;
	}

}


void FixedRecordIteraor::InitScriptVar(void *p, void * s)
{
	ACE_TRACE("(%D)(%t) FixedRecordIteraor::InitScriptVar");
	//Record::RECORDDATANAME::iterator it = _FixedLocationRecord->_recordDataName.begin();
	//for(;it!=_FixedLocationRecord->_recordDataName.end();it++)
	//{
	//	unsigned long  i=0;
	//	SQChar *varName = (SQChar *)it->field.c_str();
	//	SQChar *addr = const_cast<SQChar*>( "" ); 
	//	Sqrat::Table* pRecordTable = reinterpret_cast<Sqrat::Table *>(p);
	//	pRecordTable->SetValue( varName, addr ); 
	//}
	//Sqrat::Script* pScript = reinterpret_cast<Sqrat::Script *>(s);
//	pScript->Compile

}



void FixedRecordIteraor::setScriptVar(void *p)
{
	ACE_TRACE("(%D)(%t) FixedRecordIteraor::setScriptVar");
	RecordDetails::FIELDDETAILS::iterator it =  _RecordDetails->GetFieldDetails()->begin();
	//Record::RECORDDATANAME::iterator it = _FixedLocationRecord->_recordDataName.begin();
	for(;it!=_RecordDetails->GetFieldDetails()->end();it++)
	{
		unsigned long  i=0;
		//const char *varName = it->field.c_str();
		//char *addr = const_cast<char*>(it->cell.data()); 

		SQChar *varName = (SQChar *) it->NAME.c_str();
		SQChar *addr = const_cast<SQChar*>( it->_RecordData.cell.data()  ); 
		Sqrat::Table* pRecordTable = reinterpret_cast<Sqrat::Table *>(p);
		//pConstTable->Const( varName, addr );
		pRecordTable->SetValue( varName, addr ); 

		//int type =TCL_LINK_STRING | TCL_LINK_READ_ONLY;
		//Tcl_Interp * _ti = (Tcl_Interp *)ptcl;
		//char current_title_space[60];
		//Tcl_LinkVar(_ti, varName, (char*)&addr, type);

	}

}


