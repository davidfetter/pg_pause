# pg_pause
Hook-based system for pausing operations in PostgreSQL.

To install, add pg_pause to your shared_preload_libraries.  For
example, if you know you don't have any shared_preload_libraries, you
can do:
```
    ALTER SYSTEM SET shared_preload_libraries = 'pg_pause';
```

then restart the server.

To pause DDL, do:

```
ALTER SYSTEM SET pg_pause.ddl = true;
SELECT pg_reload_conf();
```

To resume DDL:

```
ALTER SYSTEM SET pg_pause.ddl = false;
SELECT pg_reload_conf();
```
