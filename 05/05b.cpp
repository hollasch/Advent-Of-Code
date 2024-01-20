//----------------------------------------------------------------------------------------------------------------------
// 2023 Advent of Code -- Puzzle 05b (See Part Two below)
//
// --- Day 5: If You Give A Seed A Fertilizer ---
//
// You take the boat and find the gardener right where you were told he would be: managing a giant
// "garden" that looks more to you like a farm.
//
// "A water source? Island Island is the water source!" You point out that Snow Island isn't
// receiving any water.
//
// "Oh, we had to stop the water because we ran out of sand to filter it with! Can't make snow with
// dirty water. Don't worry, I'm sure we'll get more sand soon; we only turned off the water a few
// days... weeks... oh no." His face sinks into a look of horrified realization.
//
// "I've been so busy making sure everyone here has food that I completely forgot to check why we
// stopped getting more sand! There's a ferry leaving soon that is headed over in that direction -
// it's much faster than your boat. Could you please go check it out?"
//
// You barely have time to agree to this request when he brings up another. "While you wait for the
// ferry, maybe you can help us with our food production problem. The latest Island Island Almanac
// just arrived and we're having trouble making sense of it."
//
// The almanac (your puzzle input) lists all of the seeds that need to be planted. It also lists
// what type of soil to use with each kind of seed, what type of fertilizer to use with each kind of
// soil, what type of water to use with each kind of fertilizer, and so on. Every type of seed,
// soil, fertilizer and so on is identified with a number, but numbers are reused by each category -
// that is, soil 123 and fertilizer 123 aren't necessarily related to each other.
//
// For example:
//
//     seeds: 79 14 55 13
// 
//     seed-to-soil map:
//     50 98 2
//     52 50 48
// 
//     soil-to-fertilizer map:
//     0 15 37
//     37 52 2
//     39 0 15
// 
//     fertilizer-to-water map:
//     49 53 8
//     0 11 42
//     42 0 7
//     57 7 4
// 
//     water-to-light map:
//     88 18 7
//     18 25 70
// 
//     light-to-temperature map:
//     45 77 23
//     81 45 19
//     68 64 13
// 
//     temperature-to-humidity map:
//     0 69 1
//     1 0 69
// 
//     humidity-to-location map:
//     60 56 37
//     56 93 4
//
// The almanac starts by listing which seeds need to be planted: seeds 79, 14, 55, and 13.
//
// The rest of the almanac contains a list of maps which describe how to convert numbers from a
// source category into numbers in a destination category. That is, the section that starts with
// seed-to-soil map: describes how to convert a seed number (the source) to a soil number (the
// destination). This lets the gardener and his team know which soil to use with which seeds, which
// water to use with which fertilizer, and so on.
//
// Rather than list every source number and its corresponding destination number one by one, the
// maps describe entire ranges of numbers that can be converted. Each line within a map contains
// three numbers: the destination range start, the source range start, and the range length.
//
// Consider again the example seed-to-soil map:
//
//     50 98 2
//     52 50 48
//
// The first line has a destination range start of 50, a source range start of 98, and a range
// length of 2. This line means that the source range starts at 98 and contains two values: 98 and
// 99. The destination range is the same length, but it starts at 50, so its two values are 50 and
// 51. With this information, you know that seed number 98 corresponds to soil number 50 and that
// seed number 99 corresponds to soil number 51.
//
// The second line means that the source range starts at 50 and contains 48 values: 50, 51, ..., 96,
// 97. This corresponds to a destination range starting at 52 and also containing 48 values: 52, 53,
// ..., 98, 99. So, seed number 53 corresponds to soil number 55.
//
// Any source numbers that aren't mapped correspond to the same destination number. So, seed number
// 10 corresponds to soil number 10.
//
// So, the entire list of seed numbers and their corresponding soil numbers looks like this:
//
//     seed  soil
//     0     0
//     1     1
//     ...   ...
//     48    48
//     49    49
//     50    52
//     51    53
//     ...   ...
//     96    98
//     97    99
//     98    50
//     99    51
//
// With this map, you can look up the soil number required for each initial seed number:
//
//   - Seed number 79 corresponds to soil number 81.
//   - Seed number 14 corresponds to soil number 14.
//   - Seed number 55 corresponds to soil number 57.
//   - Seed number 13 corresponds to soil number 13.
//
// The gardener and his team want to get started as soon as possible, so they'd like to know the
// closest location that needs a seed. Using these maps, find the lowest location number that
// corresponds to any of the initial seeds. To do this, you'll need to convert each seed number
// through other categories until you can find its corresponding location number. In this example,
// the corresponding types are:
//
//   - Seed 79, soil 81, fertilizer 81, water 81, light 74, temperature 78, humidity 78, location 82.
//   - Seed 14, soil 14, fertilizer 53, water 49, light 42, temperature 42, humidity 43, location 43.
//   - Seed 55, soil 57, fertilizer 57, water 53, light 46, temperature 82, humidity 82, location 86.
//   - Seed 13, soil 13, fertilizer 52, water 41, light 34, temperature 34, humidity 35, location 35.
//
// So, the lowest location number in this example is 35.
//
// What is the lowest location number that corresponds to any of the initial seed numbers?
//
// --- Part Two ---
//
// Everyone will starve if you only plant such a small number of seeds. Re-reading the almanac, it
// looks like the seeds: line actually describes ranges of seed numbers.
//
// The values on the initial seeds: line come in pairs. Within each pair, the first value is the
// start of the range and the second value is the length of the range. So, in the first line of the
// example above:
//
//     seeds: 79 14 55 13
//
// This line describes two ranges of seed numbers to be planted in the garden. The first range
// starts with seed number 79 and contains 14 values: 79, 80, ..., 91, 92. The second range starts
// with seed number 55 and contains 13 values: 55, 56, ..., 66, 67.
//
// Now, rather than considering four seed numbers, you need to consider a total of 27 seed numbers.
//
// In the above example, the lowest location number can be obtained from seed number 82, which
// corresponds to soil 84, fertilizer 84, water 84, light 77, temperature 45, humidity 46, and
// location 46. So, the lowest location number is 46.
//
// Consider all of the initial seed numbers listed in the ranges on the first line of the almanac.
// What is the lowest location number that corresponds to any of the initial seed numbers? 
//----------------------------------------------------------------------------------------------------------------------

#include <iostream>
#include <string>
#include <vector>

using namespace std;
using Value = unsigned long;


const bool debug = false;
const bool showProgress = true;


struct SeedRange {
    Value min;
    Value max;
};

struct Mapping {
    // A single source-to-destination mapping

    Value destinationRangeStart;
    Value sourceRangeStart;
    Value rangeLength;

    Value evaluate(Value source) const {
        if (sourceRangeStart <= source && source < sourceRangeStart + rangeLength)
            return source - sourceRangeStart + destinationRangeStart;
        return source;
    }

    bool read() {
        string line;
        getline(cin, line);
        if (line.empty()) return false;

        sscanf(line.c_str(), "%lu %lu %lu", &destinationRangeStart, &sourceRangeStart, &rangeLength);
        return true;
    }

    void dump() const {
        cout << destinationRangeStart << " " << sourceRangeStart << " " << rangeLength << '\n';
    }
};


Value map(const vector<Mapping>& mappings, Value x) {
    // Given a set of mappings, evaluates each in order to map from the source x to its destination.
    for (auto& mapping : mappings) {
        Value y = mapping.evaluate(x);
        if (y != x) return y;
    }
    return x;
}


struct Mappings {
    // The full set of garden mappings

    vector<Mapping> seedToSoil;
    vector<Mapping> soilToFertilizer;
    vector<Mapping> fertilizerToWater;
    vector<Mapping> waterToLight;
    vector<Mapping> lightToTemperature;
    vector<Mapping> temperatureToHumidity;
    vector<Mapping> humidityToLocation;

    Value location(Value humidity) const {
        return map(humidityToLocation, humidity);
    }

    Value humidity(Value temperature) const {
        return map(temperatureToHumidity, temperature);
    }

    Value temperature(Value light) const {
        return map(lightToTemperature, light);
    }

    Value light(Value water) const {
        return map(waterToLight, water);
    }

    Value water(Value fertilizer) const {
        return map(fertilizerToWater, fertilizer);
    }

    Value fertilizer(Value soil) const {
        return map(soilToFertilizer, soil);
    }

    Value soil(Value seed) const {
        return map(seedToSoil, seed);
    }

    void dump() const {
        cout << "\nseed-to-soil map:\n";
        for (auto mapping : seedToSoil) mapping.dump();

        cout << "\nsoil-to-fertilizer map:\n";
        for (auto mapping : soilToFertilizer) mapping.dump();

        cout << "\nfertilizer-to-water map:\n";
        for (auto mapping : fertilizerToWater) mapping.dump();

        cout << "\nwater-to-light map:\n";
        for (auto mapping : waterToLight) mapping.dump();

        cout << "\nlight-to-temperature map:\n";
        for (auto mapping : lightToTemperature) mapping.dump();

        cout << "\ntemperature-to-humidity map:\n";
        for (auto mapping : temperatureToHumidity) mapping.dump();

        cout << "\nhumidity-to-location map:\n";
        for (auto mapping : humidityToLocation) mapping.dump();
    }
};


struct GardenData {
    // The full set of garden data
    vector<SeedRange> seedRanges;
    Mappings          mappings;

    void dump() const {
        cout << "seeds:";
        for (auto& seedRange : seedRanges)
            cout << ' ' << seedRange.min << '-' << seedRange.max;
        cout << '\n';

        mappings.dump();
    }

    Value nearestLocation() const {
        // Return the nearest location (smallest value) of all seeds.

        if (debug) cout << '\n';

        unsigned long seedCount = 0;
        if (showProgress) {
            for (auto& seedRange : seedRanges)
                seedCount += seedRange.max - seedRange.min + 1;
        }

        unsigned long batchSize = seedCount / 100;
        unsigned long batchRemaining = 1;
        unsigned long percentRemaining = 100;

        if (showProgress)
            cout << seedCount << " seeds, batch size " << batchSize << '\n' << std::flush;

        bool first = true;
        Value nearest = 0;
        for (auto& seedRange : seedRanges) {
            for (auto seed = seedRange.min; seed <= seedRange.max; ++seed) {
                Value soil = mappings.soil(seed);
                Value fertilizer = mappings.fertilizer(soil);
                Value water = mappings.water(fertilizer);
                Value light = mappings.light(water);
                Value temperature = mappings.temperature(light);
                Value humidity = mappings.humidity(temperature);
                Value location = mappings.location(humidity);

                if (debug) {
                    cout << "Seed " << seed
                         << ", soil " << soil
                         << ", fertilizer " << fertilizer
                         << ", water " << water
                         << ", light " << light
                         << ", temperature " << temperature
                         << ", humidity " << humidity
                         << ", location " << location
                         << '\n';
                }

                if (first || location < nearest) {
                    nearest = location;
                    first = false;
                }

                if (showProgress) {
                    if (--batchRemaining == 0) {
                        cout << '\r' << percentRemaining << "% " << std::flush;
                        --percentRemaining;
                        batchRemaining = batchSize;
                    }
                }
            }
        }

        if (showProgress) cout << '\n';
        if (debug) cout << '\n';

        return nearest;
    }
};


void readSeeds(const string& line, GardenData& data) {
    // Read the list of seeds from the input line.

    data.seedRanges.clear();
    SeedRange seedRange;

    auto *ptr = line.c_str();
    while (*ptr) {
        while (*ptr && *ptr != ' ') ++ptr;   // Scan to next space.
        if (!*ptr) break;
        while (*ptr == ' ') ++ptr;           // Skip spaces.
        sscanf(ptr, "%lu", &seedRange.min);  // Read range start.
        
        Value length;

        while (*ptr && *ptr != ' ') ++ptr;   // Scan to next space.
        while (*ptr == ' ') ++ptr;           // Skip spaces.
        sscanf(ptr, "%lu", &length);         // Read range end.

        seedRange.max = seedRange.min + length - 1;
        data.seedRanges.push_back(seedRange);
    }
}


void readMappings(vector<Mapping>& mappings) {
    // Read a set of mappings from standard input.

    string line;
    Mapping mapping;
    while (mapping.read())
        mappings.push_back(mapping);
}


bool readSection(GardenData& data) {
    // Read a section of the input file, which is a set of mappings from one category to another.
    // Each line in the section is a mapping, and each mapping is three integers separated by
    // whitespace: the destination range start, the source range start, and the range length.

    string line;

    while (true) {
        if (!getline(cin, line)) return false;
        if (!line.empty()) break;
    }

    if (line.starts_with("seeds: "))
        readSeeds(line, data);
    else if (line == "seed-to-soil map:")
        readMappings(data.mappings.seedToSoil);
    else if (line == "soil-to-fertilizer map:")
        readMappings(data.mappings.soilToFertilizer);
    else if (line == "fertilizer-to-water map:")
        readMappings(data.mappings.fertilizerToWater);
    else if (line == "water-to-light map:")
        readMappings(data.mappings.waterToLight);
    else if (line == "light-to-temperature map:")
        readMappings(data.mappings.lightToTemperature);
    else if (line == "temperature-to-humidity map:")
        readMappings(data.mappings.temperatureToHumidity);
    else if (line == "humidity-to-location map:")
        readMappings(data.mappings.humidityToLocation);
    else {
        cout << "Unknown line: " << line << '\n';
        return false;
    }

    return true;
}


int main() {
    GardenData gardenData;

    while (readSection(gardenData))
        continue;

    if (debug) gardenData.dump();

    Value nearestLocation = gardenData.nearestLocation();

    cout << "Nearest location: " << nearestLocation << '\n';

    return 0;
}
