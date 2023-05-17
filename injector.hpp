#pragma once

#include <includes.hpp>

namespace injector {

	namespace data {
		inline HANDLE handle = INVALID_HANDLE_VALUE;
		inline uint32_t process_id = 0;
	}

	inline uint32_t find_process_id(const char* name) {

		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 pe32{ sizeof(PROCESSENTRY32) };

		Process32First(snapshot, &pe32);
		while (Process32Next(snapshot, &pe32)) {
			if (strcmp(pe32.szExeFile, name) == 0) {
				CloseHandle(snapshot);
				return static_cast<uint32_t>(pe32.th32ProcessID);
			}
		}

		CloseHandle(snapshot);
		return 0;
	}

	inline bool inject(const char* name, const char* file_path) {

		data::process_id = find_process_id(name);
		if (!data::process_id)
			return false;

		std::vector<uint8_t> buffer;
		std::ifstream file_ifstream(file_path, std::ios::binary);

		if (!file_ifstream)
			return false;

		buffer.assign((std::istreambuf_iterator<char>(file_ifstream)), std::istreambuf_iterator<char>());
		file_ifstream.close();

		data::handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, data::process_id);
		if (!data::handle || data::handle == INVALID_HANDLE_VALUE)
			return false;

		int length = strlen(file_path);

		void* allocated_memory = VirtualAllocEx(data::handle, 0, length + 1, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (!allocated_memory)
			return false;

		if (!WriteProcessMemory(data::handle, 0, file_path, (length + 1), 0))
			return false;

		HANDLE thread = CreateRemoteThread(data::handle, 0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(LoadLibraryA), allocated_memory, 0, 0);
		if (!thread || thread == INVALID_HANDLE_VALUE)
			return false;

		WaitForSingleObject(thread, INFINITE);
		VirtualFreeEx(data::handle, allocated_memory, length + 1, MEM_RELEASE);

		CloseHandle(data::handle);
		CloseHandle(thread);
		return true;
	}
}