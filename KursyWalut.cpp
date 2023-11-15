#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

struct ExchangeRate
{
    std::string date;
    double usdRate;
    double eurRate;
    std::string id;
};

// reading data from a csv file and saving it in a vector holding the ExchangeRate type.
std::vector<ExchangeRate> LoadFile(std::string &stream)
{
    std::string line;
    std::ifstream exchangeRateFile(stream);
    std::vector<ExchangeRate> allExchangeRates;

    if (exchangeRateFile)
    {
        std::getline(exchangeRateFile, line);
        while (std::getline(exchangeRateFile, line))
        {
            ExchangeRate exchaneRate;
            std::stringstream lineStream(line);

            std::getline(lineStream, exchaneRate.date, ',');

            std::string usdRate;
            std::getline(lineStream, usdRate, ',');
            exchaneRate.usdRate = std::stod(usdRate);

            std::string eurRate;
            std::getline(lineStream, eurRate, ',');
            exchaneRate.eurRate = std::stod(eurRate);

            std::getline(lineStream, exchaneRate.id, ',');

            allExchangeRates.push_back(exchaneRate);
        }
    }
    exchangeRateFile.close();

    return allExchangeRates;
}

// sorting data by dollar exchange rate
std::vector<ExchangeRate> sort_usd(std::vector<ExchangeRate> ratesToSort)
{
    for (int i = 0; i < ratesToSort.size(); i++)
    {
        for (int j = 1; j < ratesToSort.size() - i; j++)
        {
            if (ratesToSort[j - 1].usdRate > ratesToSort[j].usdRate)
            {
                std::swap(ratesToSort[j - 1], ratesToSort[j]);
            }
        }
    }

    return ratesToSort;
}

// sorting data by euro exchange rate
std::vector<ExchangeRate> sort_eur(std::vector<ExchangeRate> ratesToSort)
{
    for (int i = 0; i < ratesToSort.size(); i++)
    {
        for (int j = 1; j < ratesToSort.size() - i; j++)
        {
            if (ratesToSort[j - 1].eurRate > ratesToSort[j].eurRate)
            {
                std::swap(ratesToSort[j - 1], ratesToSort[j]);
            }
        }
    }

    return ratesToSort;
}

// printing information about all exchange rates
void PrintRates(const std::vector<ExchangeRate> &ratesToPrint)
{
    for (int i = 0; i < ratesToPrint.size(); i++)
    {
        std::cout << ratesToPrint[i].date << " " << ratesToPrint[i].usdRate << " " << ratesToPrint[i].eurRate << " " << ratesToPrint[i].id << std::endl;
    }
    std::cout << std::endl;
}

// printing the date and rate for the selected currency in the specified range
void PrintRatesRange(const std::vector<ExchangeRate> &ratesToPrint, const int &rangeBeginning, const int &rangeEnd, bool usd)
{
    int poz = 1;
    for (int i = rangeBeginning; i < rangeEnd; i++)
    {
        if (usd)
        {
            std::cout << poz << ". " << ratesToPrint[i].date << " " << ratesToPrint[i].usdRate << std::endl;
        }
        else
        {
            std::cout << poz << ". " << ratesToPrint[i].date << " " << ratesToPrint[i].eurRate << std::endl;
        }
        poz++;
    }
}

// binary search to find a given dollar rate
int SearchBinary(int l, int r, const double &searchedValue, const std::vector<ExchangeRate> &vectorToEvaluate)
{
    int sr;
    while (l <= r)
    {
        sr = (l + r) / 2;

        if (vectorToEvaluate[sr].usdRate == searchedValue)
            return sr;

        if (vectorToEvaluate[sr].usdRate > searchedValue)
            r = sr - 1;
        else
            l = sr + 1;
    }

    return -1;
}

int main()
{

    std::string fileStream = "kursy_usd_eur.csv";
    std::vector<ExchangeRate> foundExchangeRates, sortedByUsd, sortedByEur;

    foundExchangeRates = LoadFile(fileStream);
    sortedByUsd = sort_usd(foundExchangeRates);
    sortedByEur = sort_eur(foundExchangeRates);

    std::cout << "Sorted by usd \n";
    PrintRates(sortedByUsd);
    std::cout << "Sorted by eur \n";
    PrintRates(sortedByEur);

    std::cout << "Highest rates usd: \n";
    PrintRatesRange(sortedByUsd, sortedByUsd.size() - 10, sortedByUsd.size(), true);

    std::cout << "Lowest rates eur: \n";
    PrintRatesRange(sortedByEur, 0, 10, false);

    double valueToFind = 3.9011;
    int index = SearchBinary(0, sortedByUsd.size() - 1, valueToFind, sortedByUsd);
    if (index >= 0)
    {
        std::cout << "Such exchange rate for usd was on: " << foundExchangeRates[index].date << std::endl;
    }
    else
    {
        std::cout << "Value: " << valueToFind << " not found." << std::endl;
    }
}
