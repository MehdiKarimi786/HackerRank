#!/usr/bin/python -u
import argparse
import re
import os
import colorama
from colorama import Fore
from colorama import Style

parser = argparse.ArgumentParser(description='ETAS GETK CMake Build Tool')
parser.add_argument("-v", dest="verbosity", action='count', help="Verbosity")
parser.add_argument("-p", dest="previous_logfile", action="store", help="Previous Logfile", type=argparse.FileType('r'))
parser.add_argument('logfile',  type=argparse.FileType('r'))


class Entry:
    def __init__(self, entrytype, location, message, project, sourcefile, sourceline):
        self.entrytype = entrytype
        self.location = location
        self.message = message
        self.project = project
        self.sourcefile = sourcefile
        self.sourceline = sourceline

    def print(self, verbosity=0):
        colorama.init()
        if (self.project or self.location) and int(verbosity) > 0:
            msg = ""
            if self.project:
                msg += Fore.LIGHTCYAN_EX + self.project + Style.RESET_ALL + "\n"
            if self.sourcefile:
                msg += Fore.LIGHTBLACK_EX + self.sourcefile + "(" + str(self.sourceline) + ")\n" + Style.RESET_ALL
            elif self.location is not None:
                msg += Fore.LIGHTBLACK_EX + self.location + Style.RESET_ALL + "\n"
            if self.entrytype == 'warning':
                color = Fore.LIGHTYELLOW_EX
            else:
                color = Fore.RED
            msg += color + self.message + Style.RESET_ALL + "\n"
            print(msg)
        if int(verbosity) >= 2 and self.sourcefile is not None:
            fp = open(self.sourcefile.replace('\\','/'), 'r', encoding='utf-8')
            lines = fp.readlines()
            startline = max(0, self.sourceline - 3)
            endline = min(len(lines), startline+5)
            for line in lines[startline:endline]:
                print(Style.DIM + Fore.CYAN + line.strip() + Style.RESET_ALL)
            print()
            fp.close()
        colorama.deinit()
    def __eq__(self, other):
        return self.location == other.location and self.message == other.message


class ErrorSet:
    def __init__(self, log):
        self.entries = []
        fp = open(log, 'r')
        self.lines = fp.readlines()
        fp.close()
        regex = re.compile("(.+)(: fatal error|: error|: warning)[:]?[ ]*(.+)")
        for line in self.lines:
            m = regex.match(line)
            if m is not None:
                if 'error' in m.group(2):
                    t = 'error'
                else:
                    t = 'warning'
                location = m.group(1).replace("/", "\\").strip()
                message = m.group(3).strip()
                projectname = None
                sourcefile = None
                sourceline = 0

                m_prj = re.search(r"\[.*[\\|\/]([\w_\s\-]+).vcxproj\]", message)
                if m_prj is not None:
                    projectname = m_prj.group(1)
                    message = message.replace(m_prj.group(0), "")

                m_loc = re.match("^(.*):([0-9]+):([0-9]+)$", location)
                if m_loc is not None:
                    sourceline = int(m_loc.group(2))
                    sourcefile = m_loc.group(1)
                else:
                    m_loc = re.match("^(.*)\(([0-9]+)\)$", location)
                    if m_loc is not None:
                        sourceline = int(m_loc.group(2))
                        sourcefile = m_loc.group(1).strip() if os.path.exists(m_loc.group(1).strip()) else None
                if sourcefile is not None:
                    pass
                e = Entry(t, location, message, projectname, sourcefile, sourceline)
                self.entries.append(e)

    def errors(self):
        return len([e for e in self.entries if e.entrytype == 'error'])

    def warnings(self):
        return len([e for e in self.entries if e.entrytype == 'warning'])


def main(logfile, prev_logfile=None, verbosity=2):
    error_set = ErrorSet(logfile)

    for e in error_set.entries:
        if verbosity is None:
            verbosity = 0
        e.print(verbosity)
    colorama.init()
    if prev_logfile is not None and os.path.exists(prev_logfile):
        prev_error_set = ErrorSet(prev_logfile)
        print(Fore.RED + 'Errors: '+str(prev_error_set.errors())+' -> '+str(error_set.errors()) + Style.RESET_ALL)
        print(Fore.LIGHTYELLOW_EX+'Warnings: '+str(prev_error_set.warnings())+' -> '+str(error_set.warnings()) + Style.RESET_ALL)
    else:
        print(Fore.RED+'Errors: '+str(error_set.errors()) + Style.RESET_ALL)
        print(Fore.LIGHTYELLOW_EX+'Warnings: '+str(error_set.warnings()) + Style.RESET_ALL)
    colorama.deinit()


if __name__ == "__main__":
    args = parser.parse_args()
    if args.previous_logfile is not None:
        main(args.logfile.name, args.previous_logfile.name, args.verbosity)
    else:
        main(args.logfile.name, None, args.verbosity)
