#include "CharRecordIteraor.h"
#include "CharRecord.h"
#include "ace/Log_Msg.h"
//#include "cpptcl.h"
#include <sqrat.h>


CharRecordIteraor::CharRecordIteraor(RecordDetails* pRecordDetails):Recorditerator(pRecordDetails)
{
	ACE_TRACE("CharRecordIteraor::CharRecordIteraor");
	_data=0;
	_RecordNumber=0;
	_TotalRecords=0;
	_currentRowData=_data;
	_currentColData=_currentRowData;

}


CharRecordIteraor::~CharRecordIteraor(void)
{
	ACE_TRACE("CharRecordIteraor::~CharRecordIteraor");
}


// bool CharRecordIteraor::value(Record * & )
//{
//	 return false;
//}

 AR_INT64 CharRecordIteraor::Prev(void){ 

	 return Recorditerator::RECORDPOSITION::RECORDPOSITION_INVALID;

}


void CharRecordIteraor::setSTRVar(std::map<std::string, std::string> &s)
{
	ACE_TRACE("CharRecordIteraor::setSTRVar");
	RecordDetails::FIELDDETAILS::iterator it = this->_RecordDetails->GetFieldDetails()->begin();
	for(;it!=this->_RecordDetails->GetFieldDetails()->end();it++)
	{
		char *addr = const_cast<char*>(it->_RecordData.cell.data() ); 
		s[it->NAME]=addr ;
	}

}

void CharRecordIteraor::InitScriptVar(void *p, void * s)
{
	ACE_TRACE("CharRecordIteraor::InitScriptVar");
	RecordDetails::FIELDDETAILS::iterator  it = this->_RecordDetails->GetFieldDetails()->begin();
	for(;it!=this->_RecordDetails->GetFieldDetails()->end();it++)
	{
		unsigned long  i=0;
		SQChar *varName = (SQChar *)it->NAME.c_str();
		SQChar *addr = const_cast<SQChar*>( "" ); 
		Sqrat::Table* pRecordTable = reinterpret_cast<Sqrat::Table *>(p);
		pRecordTable->SetValue( varName, addr ); 
	}
	Sqrat::Script* pScript = reinterpret_cast<Sqrat::Script *>(s);
//	pScript->Compile

}

void CharRecordIteraor::setScriptVar(void * p)
{
	ACE_TRACE("CharRecordIteraor::setScriptVar");
	RecordDetails::FIELDDETAILS::iterator  it = this->_RecordDetails->GetFieldDetails()->begin();
	for( ; it != this->_RecordDetails->GetFieldDetails()->end(); it++)
	{
		unsigned long  i=0;
		SQChar *varName = (SQChar *)it->NAME.c_str();
		SQChar *addr = const_cast<SQChar*>(it->_RecordData.cell.data()); 
		Sqrat::ConstTable* pConstTable = reinterpret_cast<Sqrat::ConstTable *>(p);
		pConstTable->Const( varName, addr );
	}
}


void CharRecordIteraor::begin(void)
{
	_currentRowData=_data;
	_currentColData=_currentRowData;
	_error=CharRecordIteraor::RECORDERROR_BEGIN;

}


 AR_INT64 CharRecordIteraor::Next(void){
	ACE_TRACE("CharRecordIteraor::Next");
	PARSESTATE s;
	char * ps=_currentRowData;
	char * pe=_currentRowData;
	RecordDetails::FIELDDETAILS::iterator it  = _RecordDetails->GetFieldDetails()->begin();
	RecordDetails::FIELDDETAILS::iterator eit = _RecordDetails->GetFieldDetails()->end();

	_error=RECORDERROR::RECORDERROR_GOOD;
	if (ps >= (this->_data + this->_filesize) )	
	{
		_pos=Recorditerator::RECORDPOSITION_END;
		return _pos;
	}

	for(; it!=eit ;it++)
	{
		it->_RecordData.cell[0]=0;
	}

	int FIELDSEPERATOR = this->_RecordDetails->FIELDSEPERATOR ;
	int LINE_SEPERATOR  = this->_RecordDetails->LINE_SEPERATOR;
	for(it=_RecordDetails->GetFieldDetails()->begin(); it!=eit ;it++)
	{
		unsigned long  i=0;
		if ( (ps+i) >= (this->_data + this->_filesize) )	
		{
			_pos=Recorditerator::RECORDPOSITION_END;
			return _pos;
		}
		for(; ( ps[i] != FIELDSEPERATOR && ps[i]!= LINE_SEPERATOR) && (ps+i< (this->_data + this->_filesize)); i++ )
		{

		}
		if ( (ps+i) > (this->_data + this->_filesize) )	
		{
			//_error=RECORDERROR::RECORDERROR_BADLENGTH;
			//_pos=Recorditerator::RECORDPOSITION_INVALID;
			//return Recorditerator::RECORDERROR_BADLENGTH;
		}
		_error=RECORDERROR::RECORDERROR_GOOD;
		if(ps[i] == FIELDSEPERATOR )
			s=FIELDEND;
		if(ps[i] == LINE_SEPERATOR )
			s=LINEEND ;
		if( (ps+i)>= (this->_data + this->_filesize))
			s=FILEEND;
		if( it->_RecordData.cell.size() < (i+1) )
			it->_RecordData.cell.resize( i+1, 0);
		char * pcell = const_cast<char*>( it->_RecordData.cell.data() );		
		memcpy(pcell,ps,i);
		pcell[i]=0;
		if(ps[i] == FIELDSEPERATOR )
		{
			_currentRowData = ps + i + 1;
			ps = _currentRowData;
			continue;
		} 
		if(ps[i] == LINE_SEPERATOR) 
		{
			_currentRowData = ps + i + 1;
			ps = _currentRowData;
			if(it==eit)
				break;
			else
			{
				_error=RECORDERROR::RECORDERROR_INCOMPLETE;
				_pos=Recorditerator::RECORDPOSITION_INVALID;
				return Recorditerator::RECORDERROR_BADLENGTH;
			}
			break;
		} 
		if ( (ps+i) >= (this->_data + this->_filesize) )	
		{
			_currentRowData = ps + i + 1;
			ps = _currentRowData;
			_pos = Recorditerator::RECORDPOSITION::RECORDPOSITION_OK;
			if(it==eit)
			{
				break;
			}
			else
			{
				_RecordNumber++;
				_error=RECORDERROR::RECORDERROR_INCOMPLETE;
				_pos=Recorditerator::RECORDPOSITION_INVALID;
				return Recorditerator::RECORDERROR_BADLENGTH;
			}
			return _RecordNumber;
		}
	}
	_RecordNumber++;
	_pos = Recorditerator::RECORDPOSITION::RECORDPOSITION_OK;
	 return _RecordNumber;
}

 AR_INT64 CharRecordIteraor::getPos(void){
	 return _RecordNumber;
}

 bool CharRecordIteraor::isEnd(void ){
	 return false;
 }

 void CharRecordIteraor::goBegin(void ){
	ACE_TRACE("CharRecordIteraor::goBegin");
 	_RecordNumber=0;
	_currentRowData=_data;
	_currentColData=_currentRowData;

}

 void CharRecordIteraor::goEnd(void )
{
}

 AR_INT64 CharRecordIteraor::goTo(AR_INT64 index){
	return _RecordNumber;
}

void CharRecordIteraor::dump(void)
{
	ACE_DEBUG ((LM_DEBUG, "(%t) CharRecordIteraor \n"));
}

