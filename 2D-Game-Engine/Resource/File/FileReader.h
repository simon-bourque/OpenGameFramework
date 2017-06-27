#pragma once

#include "Core/Core.h"

#include "Core/Platform.h"

#ifdef PLATFORM_WINDOWS
#include "Windows.h"
#endif

class FileReader {
private:

	// Handles
#ifdef PLATFORM_WINDOWS
	HANDLE m_nativeHandle;
#endif

	string m_path;

	uint8* m_buffer;
	uint32 m_bufferOffset;
	uint32 m_bytesInBuffer;

	uint32 m_bufferCapacity;

	int64 m_fileSize;
	int64 m_bytesRead;

	bool m_closed;

	void readNextBuffer();
public:
	explicit FileReader(const string& path);
	virtual ~FileReader();

	bool isEndOfFile() const { return m_bytesRead >= m_fileSize; };
	bool isClosed() const { return m_closed; };

	int64 getFileSize() const { return m_fileSize; };

	void close();

	uint8 readUInt8();
	FileReader& operator>>(uint8& v);
};

