#include "CharRecordIteraor.h"
#include "CharRecord.h"
#include "ace/Log_Msg.h"
//#include "cpptcl.h"
#include <sqrat.h>


CharRecordIteraor::CharRecordIteraor(void)
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


 bool CharRecordIteraor::value(Record * & )
{
	 return false;
}

 AR_INT64 CharRecordIteraor::Prev(void){ 

	 return Recorditerator::RECORDPOSITION::RECORDPOSITION_INVALID;

}


void CharRecordIteraor::setSTRVar(std::map<std::string, std::string> &s)
{
	ACE_TRACE("FixedRecordIteraor::setSTRVar");
	Record::RECORDDATANAME::iterator it = this->_CharRecord->_recordDataName.begin();
	for(;it!=_CharRecord->_recordDataName.end();it++)
	{
		char *addr = const_cast<char*>(it->cell.data() ); 
		s[it->field]=addr ;
	}

}

void CharRecordIteraor::InitScriptVar(void *p, void * s)
{
	ACE_TRACE("FixedRecordIteraor::InitScriptVar");
	Record::RECORDDATANAME::iterator it = _CharRecord->_recordDataName.begin();
	for(;it!=_CharRecord->_recordDataName.end();it++)
	{
		unsigned long  i=0;
		SQChar *varName = (SQChar *)it->field.c_str();
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
	Record::RECORDDATANAME::iterator it = _CharRecord->_recordDataName.begin();
	for(;it!=_CharRecord->_recordDataName.end();it++)
	{
		unsigned long  i=0;
		SQChar *varName = (SQChar *)it->field.c_str();
		SQChar *addr = const_cast<SQChar*>(it->cell.data()); 
		Sqrat::ConstTable* pConstTable = reinterpret_cast<Sqrat::ConstTable *>(p);
		pConstTable->Const( varName, addr );
		//int type =TCL_LINK_STRING | TCL_LINK_READ_ONLY;
		//Tcl_Interp * _ti = (Tcl_Interp *)ptcl;
		//char current_title_space[60];
		//char *addr = it->cell.data(); 
		//Tcl_LinkVar(_ti, varName, (char*)&addr, type);
	}
}


//void CharRecordIteraor::setTCLVar(void * ptcl)
//{
//	ACE_TRACE("CharRecordIteraor::setTCLVar");
//	Record::RECORDDATANAME::iterator it = _CharRecord->_recordDataName.begin();
//	for(;it!=_CharRecord->_recordDataName.end();it++)
//	{
//		unsigned long  i=0;
//		const char *varName = it->field.c_str();
//		int type =TCL_LINK_STRING | TCL_LINK_READ_ONLY;
//		Tcl_Interp * _ti = (Tcl_Interp *)ptcl;
//		//char current_title_space[60];
//		//char *addr = it->cell.data(); 
//		//Tcl_LinkVar(_ti, varName, (char*)&addr, type);
//	}
//}

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
	Record::RECORDDATANAME::iterator it = _CharRecord->_recordDataName.begin();
	Record::RECORDDATANAME::iterator eit = _CharRecord->_recordDataName.end();

	_error=RECORDERROR::RECORDERROR_GOOD;
	if (ps >= (this->_data + this->_filesize) )	
	{
		_pos=Recorditerator::RECORDPOSITION_END;
		return _pos;
	}

	for(; it!=eit ;it++)
	{
		it->cell[0]=0;
	}
	for(it=_CharRecord->_recordDataName.begin(); it!=eit ;it++)
	{
		unsigned long  i=0;
		if ( (ps+i) >= (this->_data + this->_filesize) )	
		{
			_pos=Recorditerator::RECORDPOSITION_END;
			return _pos;
			//_pos = Recorditerator::RECORDPOSITION::RECORDPOSITION_OK;
			//return _RecordNumber;
		}
		for(; ( ps[i]!=this->FIELDSEPERATOR && ps[i]!=this->LINE_SEPERATOR) && (ps+i< (this->_data + this->_filesize)); i++ )
		{

		}
		if ( (ps+i) > (this->_data + this->_filesize) )	
		{
			//_error=RECORDERROR::RECORDERROR_BADLENGTH;
			//_pos=Recorditerator::RECORDPOSITION_INVALID;
			//return Recorditerator::RECORDERROR_BADLENGTH;
		}
		_error=RECORDERROR::RECORDERROR_GOOD;
		if(ps[i]==this->FIELDSEPERATOR)
			s=FIELDEND;
		if(ps[i]==this->LINE_SEPERATOR)
			s=LINEEND ;
		if( (ps+i)>= (this->_data + this->_filesize))
			s=FILEEND;
		if( it->cell.size() < (i+1) )
			it->cell.resize( i+1, 0);
		char * pcell = const_cast<char*>( it->cell.data() );		
		memcpy(pcell,ps,i);
		pcell[i]=0;
		if(ps[i]==this->FIELDSEPERATOR )
		{
			_currentRowData = ps + i + 1;
			ps = _currentRowData;
			//ps = _currentRowData;
			continue;
		} 
		if(ps[i]==this->LINE_SEPERATOR ) //ps[i]==this->FIELDSEPERATOR || 
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

