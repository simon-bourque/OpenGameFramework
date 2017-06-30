#include "FileReader.h"

#ifdef PLATFORM_WINDOWS

FileReader::FileReader(const string& path) : m_path(path), m_buffer(nullptr), m_bytesInBuffer(0), m_bufferCapacity(4096), m_bufferOffset(0), m_fileSize(0), m_bytesRead(0), m_closed(true) {
	m_nativeHandle = CreateFile(m_path.c_str(),
		GENERIC_READ, // Desired access
		0, // Share mode
		NULL, // Security attributes
		OPEN_EXISTING, // Creation disposition
		FILE_ATTRIBUTE_NORMAL, // Flags and attributes
		NULL // Template file
	);

	if (m_nativeHandle == INVALID_HANDLE_VALUE) {
		throw std::runtime_error("Failed to open file " + m_path);
	}

	m_closed = false;

	LARGE_INTEGER integer;
	if (!GetFileSizeEx(m_nativeHandle, &integer)) {
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