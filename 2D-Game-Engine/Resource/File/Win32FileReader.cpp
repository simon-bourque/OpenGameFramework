#include "FileReader.h"

#ifdef PLATFORM_WINDOWS

FileReader::FileReader(const string& path) : m_path(path), m_buffer(nullptr), m_bytesInBuffer(0), m_bufferCapacity(4096), m_bufferOffset(0), m_fileSize(0), m_bytesRead(0), m_closed(true) {
	m_nativeHandle = CreateFile(m_path.c_str(),
		GENERIC_READ, // Desired access
		0, // Share mode
		NULL, // Security attributes
		OPEN_EXISTING, // Creation disposition
		FILE_FLAG_SEQUENTIAL_SCAN, // Flags and attributes
		NULL // Template file
	);

	if (m_nativeHandle == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("Failed to open file " + m_path);
	}

	m_closed = false;

	LARGE_INTEGER integer;
	if (!GetFileSizeEx(m_nativeHandle, &integer)) {
		close();
		throw std::runtime_error("Failed to open file " + m_path);
	}

	m_fileSize = integer.QuadPart;

	m_buffer = new uint8[m_bufferCapacity];
}

FileReader::~FileReader() {
	close();
}

void FileReader::close() {
	if (!m_closed) {
		CloseHandle(m_nativeHandle);
		delete[] m_buffer;
		m_closed = true;
	}
}

void FileReader::read(uint8* buffer, uint32 bytesToRead, uint32& bytesRead) {
	if (isEndOfFile() || isClosed()) {
		bytesRead = 0;
		return;
	}

	uint32 bytesLeftInBuffer = m_bytesInBuffer - m_bufferOffset;

	// If all the bytes are contained in the buffer
	if (bytesToRead <= bytesLeftInBuffer) {
		memcpy(buffer, (m_buffer+m_bufferOffset), bytesToRead);
		m_bufferOffset += bytesToRead;
		bytesRead = bytesToRead;
	}
	else {
		// Read the remaining bytes in the buffer
		memcpy(buffer, (m_buffer + m_bufferOffset), bytesLeftInBuffer);
		m_bufferOffset += bytesLeftInBuffer;

		uint32 bytesLeftToRead = bytesToRead - bytesLeftInBuffer;
		
		// If the remaining bytes to read would be greater than or equal to the size of the next buffer then don't bother using the buffer
		if (bytesLeftToRead >= m_bufferCapacity) {

			BOOL result = ReadFile(m_nativeHandle, (buffer + bytesLeftInBuffer), bytesLeftToRead, (LPDWORD)(&bytesRead), NULL);

			if (result == FALSE) {
				throw std::runtime_error("Error reading file " + m_path);
			}

			bytesRead += bytesLeftInBuffer;
		}
		else {
			readNextBuffer();
			uint32 bytesToReadInNextBuffer = (m_bytesInBuffer < bytesLeftToRead) ? m_bytesInBuffer : bytesLeftToRead;

			memcpy((buffer + bytesLeftInBuffer), (m_buffer + m_bufferOffset), bytesToReadInNextBuffer);
			m_bufferOffset += bytesToReadInNextBuffer;
			bytesRead = bytesLeftInBuffer + bytesToReadInNextBuffer;
		}
	}
	m_bytesRead += bytesRead;
}

void FileReader::readNextBuffer() {
	if (m_bufferOffset >= m_bytesInBuffer) {
		BOOL result = ReadFile(m_nativeHandle, m_buffer, m_bufferCapacity, (LPDWORD)(&m_bytesInBuffer), NULL);

		if (result == FALSE) {
			throw std::runtime_error("Error reading file " + m_path);
		}

		m_bufferOffset = 0;
	}
}

#endif