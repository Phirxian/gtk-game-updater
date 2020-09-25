#include "../app.h"

#include <archive.h>
#include <archive_entry.h>
#include <string.h>

namespace tools
{
    Minitar::Minitar(context::Patcher *u) noexcept : verbose(true), parent(u)
    {
    }
    void Minitar::msg(const char *m)  noexcept
    {
        std::string log;
        log += m;
        parent->logMsg(log);
    }
    void Minitar::warn(const char *f, const char *m) noexcept
    {
        std::string log;
        log += f;
        log += " failed: ";
        log += m;
        parent->logMsg(log);
    }
    void Minitar::fail(const char *f, const char *m, int r) noexcept
    {
        std::string log;
        log += f;
        log += " failed: ";
        log += m;
        parent->logMsg(log);

        Gtk::MessageDialog dialog(*parent->app, log.c_str());
        dialog.set_modal(true);
        dialog.set_position(Gtk::WIN_POS_CENTER);
        dialog.run();
    }
    int Minitar::copy_data(struct archive *ar, struct archive *aw) noexcept
    {
        int r;
        const void *buff;
        size_t size;

        #ifdef __WIN32
            int64_t offset;
        #else
            #if ARCHIVE_VERSION >= 3000000
                int64_t offset;
            #else
                off_t offset;
            #endif
        #endif

        for (;;) {
            r = archive_read_data_block(ar, &buff, &size, &offset);

            if (r == ARCHIVE_EOF)
                return (ARCHIVE_OK);
            if (r != ARCHIVE_OK)
                return (r);

            r = archive_write_data_block(aw, buff, size, offset);

            if(r != ARCHIVE_OK)
            {
                warn("archive_write_data_block()",
                    archive_error_string(aw));
                return (r);
            }
        }
    }
    void Minitar::extract(const char *filename, int do_extract, int flags) noexcept
    {
        struct archive *a;
        struct archive *ext;
        struct archive_entry *entry;
        int r;

        a = archive_read_new();
        ext = archive_write_disk_new();
        archive_write_disk_set_options(ext, flags);
        /*
         * Note: archive_write_disk_set_standard_lookup() is useful
         * here, but it requires library routines that can add 500k or
         * more to a static executable.
         */
        archive_read_support_format_tar(a);

        if(filename != 0 && strcmp(filename, "-") == 0) filename = 0;
        if((r = archive_read_open_file(a, filename, 10240)))
            fail("archive_read_open_file()", archive_error_string(a), r);

        for (;;)
        {
            r = archive_read_next_header(a, &entry);
            if(r == ARCHIVE_EOF) break;
            if(r != ARCHIVE_OK) fail("archive_read_next_header()", archive_error_string(a), 1);
            if(verbose && do_extract) msg("x ");
            //if(verbose || !do_extract)
            //    msg(archive_entry_pathname(entry));
            if(do_extract)
            {
                r = archive_write_header(ext, entry);
                if(r != ARCHIVE_OK) warn("archive_write_header()", archive_error_string(ext));
                else
                {
                    copy_data(a, ext);
                    r = archive_write_finish_entry(ext);
                    if(r != ARCHIVE_OK) fail("archive_write_finish_entry()", archive_error_string(ext), 1);
                }
            }
            if(verbose || !do_extract) msg("\n");
        }
        archive_read_close(a);
        archive_read_finish(a);
    }
    void Minitar::setVerboseDebug(bool i) noexcept
    {
        verbose = i;
    }
    bool Minitar::isVerboseDebug() const noexcept
    {
        return verbose;
    }
}
