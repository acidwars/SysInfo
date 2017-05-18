#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <sys/sysinfo.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <libnet.h>
#include <thread>
#include <sys/statvfs.h>


using namespace std;


passwd getInfo() {

    int s;
    passwd pwd;
    passwd *result;
    char *buf;
    size_t bufsize;
    uid_t uid = getuid();
    bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
    buf = (char *) malloc(bufsize);
    if (!buf) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    s = getpwuid_r(uid, &pwd, buf, bufsize, &result);
    if (!result) {
        exit(EXIT_FAILURE);
    }
    return pwd;

}



void getIp(){
    ifaddrs *ifap, *ifa;
    getifaddrs(&ifap);
    sockaddr_in *sa;
    char *addr;
    /* Output! */
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family == AF_INET6) {
            sa = (sockaddr_in *) ifa->ifa_addr;
            addr = inet_ntoa(sa->sin_addr);
            cout << ifa->ifa_name << ":\t" << addr << endl;
        }
    }


}



void createFile(const char *filename) {
    int fd;
    write(fd, "test", sizeof("test")-1);
    close(fd);
}

int main(int argc, char *argv[]) {
    struct statvfs vfs;
    statvfs("/dev/sda", &vfs);
    const long minute = 60;
    const long hour = minute * 60;
    const long day = hour * 24;
    const double megabyte = 1024 * 1024;
    struct sysinfo si;
    sysinfo(&si);
    char info;
    passwd userInfo = getInfo();
    char hostname[10];
    gethostname(hostname, sizeof(hostname));
    getIp();
    cout << "Hostname:\t\t" << hostname << endl;
    cout << "UID:\t\t\t" << userInfo.pw_uid << endl;
    cout << "Username:\t\t" << userInfo.pw_name << endl;
    cout << "Home Directory:\t\t" << userInfo.pw_dir << endl;
    cout << "Shell:\t\t\t" << userInfo.pw_shell << endl;
    cout << "Total RAM:\t\t" << si.totalram / 1024 / 1024 << "MB" << endl;
    cout << "Free  RAM:\t\t" << si.freeram / 1024 / 1024 << "MB" <<  endl;
    unsigned concurrentThreadsSupported = thread::hardware_concurrency();
    cout << "Threads Supported:\t" << concurrentThreadsSupported << endl;
    cout << "Used Disk Space:\t" << vfs.f_bsize * vfs.f_favail << endl;

}