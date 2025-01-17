/**
 * @file terminal.h
 * @author junjun.dong (dongjunjun@gaozhe.com.cn)
 * @brief 
 * @version 0.1
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef LOGGING_TERMINAL_H_
#define LOGGING_TERMINAL_H_

#ifndef LOG_NOCOLOR

#define GzReset "\x1b[0m"     // Text Reset

// Regular Colors
#define GzBlack "\x1b[30m"   // Black
#define GzCoal "\x1b[90m"    // Black
#define GzGray "\x1b[37m"    // White
#define GzWhite "\x1b[97m"   // White
#define GzMaroon "\x1b[31m"  // Red
#define GzRed "\x1b[91m"     // Red
#define GzGreen "\x1b[32m"   // Green
#define GzLime "\x1b[92m"    // Green
#define GzOrange "\x1b[33m"  // Yellow
#define GzYellow "\x1b[93m"  // Yellow
#define GzNavy "\x1b[34m"    // Blue
#define GzBlue "\x1b[94m"    // Blue
#define GzViolet "\x1b[35m"  // Purple
#define GzPurple "\x1b[95m"  // Purple
#define GzTeal "\x1b[36m"    // Cyan
#define GzCyan "\x1b[96m"    // Cyan

#define GzBlackBold "\x1b[1;30m"   // Black
#define GzCoalBold "\x1b[1;90m"    // Black
#define GzGrayBold "\x1b[1;37m"    // White
#define GzWhiteBold "\x1b[1;97m"   // White
#define GzMaroonBold "\x1b[1;31m"  // Red
#define GzRedBold "\x1b[1;91m"     // Red
#define GzGreenBold "\x1b[1;32m"   // Green
#define GzLimeBold "\x1b[1;92m"    // Green
#define GzOrangeBold "\x1b[1;33m"  // Yellow
#define GzYellowBold "\x1b[1;93m"  // Yellow
#define GzNavyBold "\x1b[1;34m"    // Blue
#define GzBlueBold "\x1b[1;94m"    // Blue
#define GzVioletBold "\x1b[1;35m"  // Purple
#define GzPurpleBold "\x1b[1;95m"  // Purple
#define GzTealBold "\x1b[1;36m"    // Cyan
#define GzCyanBold "\x1b[1;96m"    // Cyan

// Background
#define GzOnBlack "\x1b[40m"    // Black
#define GzOnCoal "\x1b[100m"    // Black
#define GzOnGray "\x1b[47m"     // White
#define GzOnWhite "\x1b[107m"   // White
#define GzOnMaroon "\x1b[41m"   // Red
#define GzOnRed "\x1b[101m"     // Red
#define GzOnGreen "\x1b[42m"    // Green
#define GzOnLime "\x1b[102m"    // Green
#define GzOnOrange "\x1b[43m"   // Yellow
#define GzOnYellow "\x1b[103m"  // Yellow
#define GzOnNavy "\x1b[44m"     // Blue
#define GzOnBlue "\x1b[104m"    // Blue
#define GzOnViolet "\x1b[45m"   // Purple
#define GzOnPurple "\x1b[105m"  // Purple
#define GzOnTeal "\x1b[46m"     // Cyan
#define GzOnCyan "\x1b[106m"    // Cyan

// Underline
#define GzBlackUnder "\x1b[4;30m"   // Black
#define GzGrayUnder "\x1b[4;37m"    // White
#define GzMaroonUnder "\x1b[4;31m"  // Red
#define GzGreenUnder "\x1b[4;32m"   // Green
#define GzOrangeUnder "\x1b[4;33m"  // Yellow
#define GzNavyUnder "\x1b[4;34m"    // Blue
#define GzVioletUnder "\x1b[4;35m"  // Purple
#define GzTealUnder "\x1b[4;36m"    // Cyan

#else
#define GzReset

// Regular Colors
#define GzBlack
#define GzCoal
#define GzGray
#define GzWhite
#define GzMaroon
#define GzRed
#define GzGreen
#define GzLime
#define GzOrange
#define GzYellow
#define GzNavy
#define GzBlue
#define GzViolet
#define GzPurple
#define GzTeal
#define GzCyan

#define GzBlackBold
#define GzCoalBold
#define GzGrayBold
#define GzWhiteBold
#define GzMaroonBold
#define GzRedBold
#define GzGreenBold
#define GzLimeBold
#define GzOrangeBold
#define GzYellowBold
#define GzNavyBold
#define GzBlueBold
#define GzVioletBold
#define GzPurpleBold
#define GzTealBold
#define GzCyanBold

// Background
#define GzOnBlack
#define GzOnCoal
#define GzOnGray
#define GzOnWhite
#define GzOnMaroon
#define GzOnRed
#define GzOnGreen
#define GzOnLime
#define GzOnOrange
#define GzOnYellow
#define GzOnNavy
#define GzOnBlue
#define GzOnViolet
#define GzOnPurple
#define GzOnTeal
#define GzOnCyan

// Underline
#define GzBlackUnder
#define GzGrayUnder
#define GzMaroonUnder
#define GzGreenUnder
#define GzOrangeUnder
#define GzNavyUnder
#define GzVioletUnder
#define GzTealUnder
#endif  // LOG_NOCOLOR

#endif  // LOGGING_TERMINAL_H_
