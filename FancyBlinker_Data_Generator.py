import math


class GammaTableCalc:
    def __init__(self, minimum=0x01, top=0x0150, gamma=2.2, off_cycles=75, on_cycles=75, flash_cycles=32,
                 beeper_frequency_hz=440):
        bits_in = 8
        self.indices = math.floor(math.pow(2, bits_in))
        self.store_in_flash = False
        self.min_value = minimum
        self.top = top
        self.gamma = gamma
        self.beeper_freq = beeper_frequency_hz
        self.off_cycles = off_cycles
        self.on_cycles = on_cycles
        self.flash_cycles = flash_cycles
        self.output = []
        self.check_arguments()

    def check_arguments(self):

        # errors
        if self.min_value < 0:
            raise ValueError("Minimum timer value must be greater or equal to 0!")
        if self.top > 0xFFFE:
            raise ValueError("Timer TOP value must be smaller than 0xFFFE!")
        if self.gamma < 0:
            raise ValueError("Gamma must be greater or equal to 0!")
        if self.off_cycles <= 0:
            raise ValueError("Off cycles must be greater than 0!")
        if self.on_cycles <= 0:
            raise ValueError("On cycles must be greater than 0!")
        if self.flash_cycles <= 0:
            raise ValueError("Flash cycles must be greater than 0!")
        if self.beeper_freq < 0:
            raise ValueError("Beeper frequency must be 0 (to disable beeper functionality) or greater than 0 (to "
                             "enable beeper)!")

        # warnings
        if self.min_value == 0:
            print("WARNING: Minimum timer value set to 0. This is likely to cause output glitches.")

        if self.gamma == 0:
            print("WARNING: Gamma correction set to 0. No correction will be performed.")

        if self.beeper_freq > 0 and self.beeper_freq < 31:
            print("WARNING: Beeper frequency is set to less than 31 Hz. According to the Arduino tone() "
                  "documentation, this is not possible. Please refer to Arduino docs for more information.")

        # information
        if self.beeper_freq > 0:
            print("Beeper functionality enabled.")
        else:
            print("Beeper functionality disabled.")

    def calc(self):
        # calculate parameters
        max_index = self.indices - 1
        max_value = self.top

        # calculate values
        for index in range(0, max_index + 1):
            value = math.floor(max_value * math.pow(index / max_index, self.gamma))
            # clamp if necessary
            if value < self.min_value:
                value = self.min_value
            self.output.append(value)

        return self.output

    def build_struct_declaration(self):
        result = "// Data Container\n" \
                 "struct FancyBlinker_Data_Struct\n" \
                 "{\n" \
                 f"\tuint16_t pwmData[{self.indices}];\n" \
                 "\tuint16_t timerTop;\n" \
                 "\tuint8_t holdOffCycles;\n" \
                 "\tuint8_t holdOnCycles;\n" \
                 "\tuint8_t flashCycles;\n"

        # add beeper freq member only if beeper is enabled
        if self.beeper_freq > 0:
            result += "\tuint16_t beeperFreq;\n"

        result += "};\n"

        return result

    def output_to_string(self, print_to_stdout=False):
        # if data has not been calculated yet, do so
        if not self.output:
            self.calc()

        # write file head
        result = "/* PWM values and constants for FancyBlinker */\n\n"
        result += "#ifndef FANCYBLINKER_DATA_H\n"
        result += "#define FANCYBLINKER_DATA_H\n"
        result += "\n"

        # write defines
        result += "#define FB_BEEPER_ENABLED "
        result += "1" if self.beeper_freq > 0 else "0"
        result += "\n\n"

        # write data structures
        result += self.build_struct_declaration()
        result += "\n// Generated Data\n"
        result += "static const FancyBlinker_Data_Struct FancyBlinker_Data"
        if self.store_in_flash:
            result += " PROGMEM"
        result += " =\n{\n"
        result += "\tpwmData: {\n\t"

        # add gamma values
        wraparound_max = 10
        wraparound_counter = 0
        for value in self.output:
            result += "\t{0},".format(value)

            # add extra tab for small numbers
            if value < 100:
                result += "\t"
            wraparound_counter += 1
            if wraparound_counter >= wraparound_max:
                result += "\n\t"
                wraparound_counter = 0
        result += "\n\t},\n"

        # add other constants
        result += "\ttimerTop:"
        result += "\t\t{0},\n".format(self.top)

        result += "\tholdOffCycles:"
        result += "\t{0},\n".format(self.off_cycles)
        result += "\tholdOnCycles:"
        result += "\t{0},\n".format(self.on_cycles)
        result += "\tflashCycles:"
        result += "\t{0},\n".format(self.flash_cycles)

        # add beeper freq member only if beeper is enabled
        if self.beeper_freq > 0:
            result += "\tbeeperFreq:"
            result += "\t\t{0}\n".format(self.beeper_freq)

        # add tail
        result += "};\n\n"
        result += "#endif\n"

        # return result and print
        if print_to_stdout:
            print(result)
        return result

    def output_to_file(self, path):
        with open(path, "w+") as file:
            content = self.output_to_string()
            file.write(content)


# entry point for standalone execution
if __name__ == "__main__":
    generator = GammaTableCalc()
    generator.output_to_file("FancyBlinker_Data.hpp")
    print("FancyBlinker_Data.hpp generated.")
