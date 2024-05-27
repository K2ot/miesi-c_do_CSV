// miesiąc_do_CSV.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "../../BotBin/BotBinanse/Sql.h"
#include <random>

static unsigned int randomInt(int64_t min, int64_t max)
{
	// Tworzymy silnik losujący (można go umieścić poza funkcją, jeśli wykorzystywany jest często)
	std::random_device rd;  // Używamy random_device do uzyskania prawdziwie losowego ziarna
	std::mt19937 gen(rd()); // mt19937 to silnik generujący liczby pseudolosowe (Mersenne Twister)

	// Definiujemy rozkład, w tym przypadku równomierny dla liczb całkowitych
	std::uniform_int_distribution<> distrib(min, max);

	// Zwracamy losową liczbę
	unsigned int ramdomInt = distrib(gen);
	return ramdomInt;
}


int main()
{
	setlocale(LC_CTYPE, "Polish");

	// Parametry do połączenia z bazą danych
	std::string host = "localhost";  // Adres serwera bazy danych
	int port = 33060;                 // Port MySQLx
	std::string user = "Admin";       // Nazwa użytkownika
	std::string password = "1234";  // Hasło użytkownika
	std::string database = "binance_data";    // Nazwa bazy danych

	// Tworzenie obiektu klasy MySQLConnector
	MySQLConnector dbConnector(host, port, user, password, database);

	const std::string& tableName{ "ethpln" };

	int64_t maxTime = dbConnector.fetchMaxOpenTime(tableName);
	int64_t minTime = dbConnector.fetchMinOpenTime(tableName);

	std::cout << "max time: " << maxTime << std::endl;
	std::cout << "min time: " << minTime << std::endl;



	int64_t randomMiesiac = randomInt((minTime / 1000), (maxTime / 1000) - 44640);

	randomMiesiac = 1666159200;

	std::cout << "randomMiesiac: " << randomMiesiac << std::endl;

	std::string csvPath{ R"(../../../Miesiace/)" + tableName + R"(_)" + std::to_string(randomMiesiac) + R"(.csv)" };

	std::ofstream csvFile(csvPath);

	if (!csvFile.is_open())
	{
		std::cerr << "Nie można otworzyć pliku CSV: " << csvPath << std::endl;
		return 0;
	}
	else
	{
		std::cerr << "Otwarty plik CSV: " << csvPath << std::endl;
	}
	
	dbConnector.dataToCSV(tableName ,randomMiesiac, csvFile);


	if (csvFile.is_open())
	{
		csvFile.close();
	}
}


	//	SELECT*	FROM nazwa_tabeli	WHERE open_time BETWEEN start_time AND end_time ORDER BY open_time ASC;

		//44 640 minut w miesiącu
//	SELECT *	FROM market_data WHERE open_time >= starting_open_time ORDER BY open_time ASC LIMIT 44640;

//	dbConnector.executeQuery