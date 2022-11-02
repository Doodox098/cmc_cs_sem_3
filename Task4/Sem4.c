// Virtual File System (VFS)
// [Disk] <=> Block Dev Driver <=> FS Driver <=> SysCall Face <=> User Space
// ext4, exfat, ntfs, ffs
// модель Posix FS
// |SUPER_BLOCK|INODES|DATA]
// INODS - массив индексных дескрипторов (метаинформация о файле)
// DATA - file content, directory content, block indirections
// Жесткая связь
// Символическая связь
// lstat - stat структура (для исследования файлововй системы)
// stat для работы с файлами
//

//    struct stat {
//        mode_t st_mode;   /* тип файла и режим (права доступа) */
//        ino_t st_ino;     /* номер индексного узла */
//        dev_t st_dev;     /* номер устройства (файловой системы) */
//        dev_t st_rdev;    /* номер устройства для специальных файлов */
//        nlink_t st_nlink; /* количество ссылок */
//        uid_t st_uid;     /* идентификатор пользователя владельца */
//        gid_t st_gid;     /* идентификатор группы владельца */
//        off_t st_size;    /* размер в байтах, для регулярных файлов */
//        time_t st_atime;  /* время последнего обращения к файлу */
//        time_t st_mtime;  /* время последнего изменения файла */
//        time_t st_ctime;  /* время последнего изменения флагов состояния файла */
//    };


    #include <sys/types.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <stdio.h>
    #include <dirent.h> // работа с каталогами

    int main(int argc, char *argv[])
    {
        char *dir = argv[1];
        DIR *d = opendir(argv[1]);
        struct dirent *dd;
        while ((dd = readdir(d)) != NULL) {
            char *name = dd->d_name;
            char *path = NULL;
            asprintf(&path, "%s/%s", dir, name);
            //printf("name: %s, ino: %lu\n", dd->d_name, dd->d_ino);
            /*struct stat stb;
            if (lstat(argv[i], &stb) >= 0) {
                printf("dev: %lu, ino: %lu, type: %x, links: %d, name: %s\n",
                       stb.st_dev, stb.st_ino, (stb.st_mode & S_IFMT),
                       stb.st_nlink, argv[i]);
            }
            if (S_ISDIR(stb.st_mode)) {
                printf("directory\n");
            } else if (S_ISLNK(stb.st_mode)) {
                printf("symlink\n");
            } else if (S_ISREG(stb.st_mode)) {
                printf("file\n");
            }*/
        }
        closedir(d);
    }
// PATH_MAX = 4Kb
//

