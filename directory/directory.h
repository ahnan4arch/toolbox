#pragma once


extern "C"{
	struct dirent;
}

namespace wOToolbox{

	class Directory{
		Directory(const Directory&);
		void operator=(const Directory&);
	public:
		class ITOR{
			ITOR();
			ITOR(const ITOR&);
			void operator=(const ITOR&);
		public:
			ITOR(Directory&);
			void operator++();
			bool IsEnd(){ return index < 0 ||  dir.numOfEntries <= index; };
			const char* Name();
			bool IsDir();
		private:
			int index;
			Directory& dir;
		};

		Directory(const char* path="./");
		~Directory();
	private:
		dirent** entries;
		int numOfEntries;

		static int DefaultFilter(const dirent*);
	};

}
