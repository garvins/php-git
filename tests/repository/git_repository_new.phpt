--TEST--
Check returned resource type of git_repository_new
--SKIPIF--
<?php if (!extension_loaded("git2")) print "skip"; ?>
--FILE--
<?php
    $test = git_repository_new();
    echo get_resource_type($test);
--EXPECT--
git2
