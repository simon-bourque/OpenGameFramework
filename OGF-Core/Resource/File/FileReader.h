#pragma once

#include "Core/Core.h"

#include "Core/Platform.h"

#include <type_traits>

#ifdef USING_WIN32_FILES
#include "Windows.h"
#endif

class FileReader {
private:

	// Handles
#ifdef USING_WIN32_FILES
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

	FileReader(const FileReader&) = delete;
	FileReader& operator=(const FileReader&) = delete;

	bool isEndOfFile() const { return m_bytesRead >= m_fileSize; };
	bool isClosed() const { return m_closed; };

	int64 getFileSize() const { return m_fileSize; };

	void close();

	void FileReader::read(uint8* buffer, uint32 bytesToRead, uint32& bytesRead);

	template<typename T>
	typename std::enable_if<((sizeof(T) > 1) && std::is_arithmetic<T>::value), T>::type read() {
		if (m_bytesRead + sizeof(T) > m_fileSize || isClosed()) {
			return 0;
		}

		uint8 bytes[sizeof(T)];
		uint32 bytesRead;

		read(bytes, sizeof(T), bytesRead);

		T v;
		memcpy(&v, bytes, sizeof(T));
		return v;
	}


	template<typename T>
	typename std::enable_if<((sizeof(T) <= 1) && std::is_arithmetic<T>::value), T>::type read() {
		if (isEndOfFile() || isClosed()) {
			return 0;
		}

		readNextBuffer();

		T v = m_buffer[m_bufferOffset++];
		++m_bytesRead;

		return v;
	}
};
