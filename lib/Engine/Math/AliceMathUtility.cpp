#include "AliceMathUtility.h"

namespace AliceMathUtility
{
#pragma region 絶対値の計算

	//値を整数に
	int Abs(int abs)
	{
		return 	std::abs(abs);
	}

	//値を整数に
	long AbsL(long abs)
	{
		return 	std::labs(abs);
	}

	//値を整数に
	long long AbsLL(long long abs)
	{
		return 	std::llabs(abs);
	}

#pragma endregion 絶対値の計算

	// ディレクトリからファイル名一覧を所得
	std::vector<std::string> getFileNames(std::string folderPath)
	{
		{
			std::filesystem::directory_iterator iter(folderPath), end;
			std::error_code err;
			std::vector<std::string> file_names;

			for (; iter != end && !err; iter.increment(err))
			{
				const std::filesystem::directory_entry entry = *iter;

				file_names.push_back(entry.path().string());
			}

			return file_names;
		}
	}
}