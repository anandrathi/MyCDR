#include "Record.h"
#include "CDRTypes.h"

#include "ace/Log_Msg.h"

void RecordDetails::AddFIXField( std::string & NAME , int START , int END , int iseq)
{
	Fielddetails fd;

	fd.NAME=NAME;
	fd.START=START;
	fd.END=END;
	fd.seq = iseq;
	if( (END-START) < 0 )
	{
		std::string expstr = "AddFIXField END-START < 0 " ; 
		expstr += "Name="  + NAME;
		std::exception e(expstr.c_str());
		throw ( e);
	}
	fd._RecordData.cell.resize(1 + 1 + END-START, 0);
	fd._RecordData.len = (END-START)+1;
	_Fielddetails.push_back(fd);

}
void RecordDetails::AddCVSField( std::string & NAME , int POS, int LEN , int iseq)
{
	Fielddetails fd;
	fd.NAME=NAME;
	fd.POS=POS;
	fd.LEN=LEN;
	fd.seq = iseq;
	_Fielddetails.push_back(fd);
}

void RecordDetails::dump(std::ostream & os)
{
	os << " PATH: (" << PATH << ")" ;
	os << " FILE_EXP: (" << FILE_EXP << ")" ;
	os << " ELEMENT_NAME: (" << ELEMENT_NAME << ")" ;
	os << " TYPE: (" << TYPE << ")" ;
	os << " SCRIPT: (" << SCRIPT << ")" ;
	os << " LOGLEVEL: (" << LOGLEVEL << ")" ;

	os << " \t FIELD_DETAILS " ;
	os << " \t BUFFER_SIZE: (" << BUFFER_SIZE << ")" ;
	os << " \t FIELDSEPERATOR: (" << FIELDSEPERATOR << ")" ;
	os << " \t LINE_SEPERATOR: (" << LINE_SEPERATOR << ")" ;
	os << " \t FIELDS: (" << FIELDS << ")" ;
	os << " \t MAX_FIELD_LEN: (" << MAX_FIELD_LEN << ")" ;
	os << " \t NUMBER_OF_FIELDS: (" << NUMBER_OF_FIELDS << ")" ;

	os << " \t INFIELDS " ;
	std::vector<Fielddetails>::iterator it = _Fielddetails.begin();
	for(; it!= _Fielddetails.end() ; it++)
	{
		os << " \t [============================================]" ;
		os << " \t NAME: (" << it->NAME << ")" ;
		os << " \t POS: (" << it->POS << ")" ;
		os << " \t LEN: (" << it->LEN << ")" ;
		os << " \t POS: (" << it->START << ")" ;
		os << " \t LEN: (" << it->END << ")" ;
		os << " \t mapto_fieldid: (" << it->mapto_fieldid << ")" ;

		os << " \t RecordData.cell: (" << it->_RecordData.cell << ")" ;
		os << " \t RecordData.cell: (" << it->_RecordData.field << ")" ;
		os << " \t RecordData.cell: (" << it->_RecordData.len << ")" ;
		os << " \t RecordData.cell: (" << (unsigned long)it->_RecordData._recposval << ")" ;
		os << " \t [############################################]" ;
	}
}
