#include "PrimitiveObjectsWriter.h"
#include "SafeBufferMacrosDefs.h"
#include "IByteWriter.h"

PrimitiveObjectsWriter::PrimitiveObjectsWriter(IByteWriter* inStreamForWriting)
{
	mStreamForWriting = inStreamForWriting;
}

PrimitiveObjectsWriter::~PrimitiveObjectsWriter(void)
{
}

static const IOBasicTypes::Byte scSpace[] = {' '};
void PrimitiveObjectsWriter::WriteTokenSeparator(ETokenSeparator inSeparate)
{
	if(eTokenSeparatorSpace == inSeparate)
		mStreamForWriting->Write(scSpace,1);
	else if(eTokenSeparatorEndLine == inSeparate)
		EndLine();
}

static const IOBasicTypes::Byte scNewLine[2] = {'\r','\n'};
void PrimitiveObjectsWriter::EndLine()
{
	mStreamForWriting->Write(scNewLine,2);
}

void PrimitiveObjectsWriter::WriteKeyword(const string& inKeyword)
{
	mStreamForWriting->Write((const IOBasicTypes::Byte *)inKeyword.c_str(),inKeyword.size());
	EndLine();
}

static const IOBasicTypes::Byte scSlash[1] = {'/'};
void PrimitiveObjectsWriter::WriteName(const string& inName,ETokenSeparator inSeparate)
{
	mStreamForWriting->Write(scSlash,1);
	mStreamForWriting->Write((const IOBasicTypes::Byte *)inName.c_str(),inName.size());
	WriteTokenSeparator(inSeparate);
}

void PrimitiveObjectsWriter::WriteInteger(long long inIntegerToken,ETokenSeparator inSeparate)
{
	char buffer[512];

	SAFE_SPRINTF_1(buffer,512,"%lld",inIntegerToken);
	mStreamForWriting->Write((const IOBasicTypes::Byte *)buffer,strlen(buffer));
	WriteTokenSeparator(inSeparate);
}

static const IOBasicTypes::Byte scLeftParanthesis[1] = {'('};
static const IOBasicTypes::Byte scRightParanthesis[1] = {')'};
void PrimitiveObjectsWriter::WriteLiteralString(const string& inString,ETokenSeparator inSeparate)
{
	mStreamForWriting->Write(scLeftParanthesis,1);
	mStreamForWriting->Write((const IOBasicTypes::Byte *)inString.c_str(),inString.size());
	mStreamForWriting->Write(scRightParanthesis,1);
	WriteTokenSeparator(inSeparate);
}

void PrimitiveObjectsWriter::WriteDouble(double inDoubleToken,ETokenSeparator inSeparate)
{
	char buffer[512];

	SAFE_SPRINTF_1(buffer,512,"%lf",inDoubleToken);

	LongBufferSizeType sizeToWrite = DetermineDoubleTrimmedLength(buffer);

	mStreamForWriting->Write((const IOBasicTypes::Byte *)buffer,sizeToWrite);
	WriteTokenSeparator(inSeparate);
}

size_t PrimitiveObjectsWriter::DetermineDoubleTrimmedLength(const char* inBufferWithDouble)
{
	size_t result = strlen(inBufferWithDouble);

	// remove all ending 0's
	while(result > 0 && inBufferWithDouble[result-1] == '0')
		--result;

	// if it's actually an integer, remove also decimal point
	if(result > 0 && inBufferWithDouble[result-1] == '.')
		--result;
	return result;
}

void PrimitiveObjectsWriter::SetStreamForWriting(IByteWriter* inStreamForWriting)
{
	mStreamForWriting = inStreamForWriting;
}

static const IOBasicTypes::Byte scOpenBracketSpace[2] = {'[',' '};
void PrimitiveObjectsWriter::StartArray()
{
	mStreamForWriting->Write(scOpenBracketSpace,2);
}

static const IOBasicTypes::Byte scCloseBracket[1] = {']'};
void PrimitiveObjectsWriter::EndArray(ETokenSeparator inSeparate)
{
	mStreamForWriting->Write(scCloseBracket,1);
	WriteTokenSeparator(inSeparate);
}