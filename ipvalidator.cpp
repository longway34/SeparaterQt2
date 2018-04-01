#include "ipvalidator.h"

IpValidator::IpValidator():QRegExpValidator(NULL)
{
     ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
     portRange = "([1-9]?[0-9]*)";
     /* Создаем регулярное выражение с применением строки, как
      * повторяющегося элемента
      */
     QRegExp ipRegex ("^" + ipRange
                      + "\\." + ipRange
                      + "\\." + ipRange
                      + "\\." + ipRange
//                      + "\\:" + portRange
                      + "$");
    this->setRegExp(ipRegex);
}
