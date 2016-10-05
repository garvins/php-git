<?php
// slipshod template generator.
//
// flag:
//   0 => generate header
//   1 => generate source
//
// --filter:
//   - prefix: don't show matched keywords
//             e.g ) -cred
//                   will ignore codes which have `cred` keyword
//   default: show matched keywords only
//             e.g ) cred
//                   will show codes which have `cred` keyword
//
// --update
//      will generate functions in order of output-file it still exists
//
// php gen.php libgit2/include/git2/<target>.h (0|1) --update --filter=[filter] > target.h or target.c

$buffer = "";

foreach ($_SERVER['argv'] as $id => $arg) {
    $_SERVER['argv'][$id] = ltrim($arg, '--');
}

if (is_dir($_SERVER['argv'][1])) {
    foreach (scandir($_SERVER['argv'][1]) as $file) {
        if (!preg_match('/(\w.h)$/' , $file) || $file == "odb_backend.h") {
           continue;
        }

        echo "Start parsing file $file .........";
        $data = parseFile($_SERVER['argv'][1] . '/' . $file);

        if ($file == "odb.h" && file_exists($_SERVER['argv'][1] . "/odb_backend.h")) {
            echo " odb_backend file found!!!";
            $data2 = parseFile($_SERVER['argv'][1] . "/odb_backend.h");
            $data = array_merge($data, $data2);
        }

        if (file_exists($_SERVER['argv'][1] . '/sys/' . $file)) {
            echo " sys file found!!!";
            $data2 = parseFile($_SERVER['argv'][1] . '/sys/' . $file);
            $data = array_merge($data, $data2);
        }

        $data2 = array();
        foreach ($data as $funcName => $func) {
            $fileBaseName = explode("_", $funcName);

            if ($fileBaseName[0] == "giterr") {
                $fileBaseName = $fileBaseName[0];
            } else if ($fileBaseName[1] == "smart") {
                $fileBaseName = "transport";
            } else {
                $fileBaseName = $fileBaseName[1];
            }

            $data2[$fileBaseName][$funcName] = $func;
        }

        if (count($data2)) {
            echo "\n";
            foreach ($data2 as $fileBaseName => $fileData) {
                $newFile = "$fileBaseName." . ($_SERVER['argv'][2] == 0 ? 'h' : 'c');

                echo "\t" . count($fileData) ." $fileBaseName-functions found.\n";
                printFile($fileData, $newFile);
            }
        } else {
            echo " nothing found!!!\n";
        }
    }
} else if (is_file($_SERVER['argv'][1])) {
    $newFile = array_pop(explode('/', $_SERVER['argv'][1]));

    printFile(parseFile($_SERVER['argv'][1]), $newFile);
}

function parseFile($path){
    $data = file_get_contents($path);

    $table = array();
    if (preg_match_all("/GIT_EXTERN\((.+?)\)\s*([a-zA-Z0-9_-]+)\(([\s\S]+?)\);/s", $data, $match)) {

        for ($i = 0; $i < count($match[0]); $i++) {
            $tmp = array(
                "args" => array(),
            );

            //replace inline callbacks
            $match[3][$i] = preg_replace("/((?:^|,)\s+)\w+\s+\(\*c(?:b|allback)\)\s*\([\s\S]*?\)(\s*(?:,|$))/", "$1{$match[2][$i]}_cb cb$2", $match[3][$i]);
            $list = array_map("trim", explode(",", $match[3][$i]));

            $pl = substr_count($match[1][$i], '*');
            $tmp['name'] = $match[2][$i];
            $tmp['rettype'] = $match[1][$i];
            $tmp['retval'] = array(
                "type" => trim(str_replace("*", "", $match[1][$i])),
                "pointer" => $pl
            );

            $d = count($list);
            if ((preg_match("/(\\*\\*|out)/", $list[0]) || preg_match("/(write|create|new)/", $match[2][$i]))) {
                $d--;
            }

            if (isset($_SERVER['argv'][3])) {
                /*if ($_SERVER['argv'][3][0] == "-") {
                    $_SERVER['argv'][3] = substr($_SERVER['argv'][3][0], 1);
                    $flag = true;
                } else {
                    $flag = false;
                }

                if (preg_match("/{$_SERVER['argv'][3]}/", $match[2][$i]) == $flag) {
                    continue;
                }*/
            }

            $match[3][$i] = trim(preg_replace("/\r?\n/", "", $match[3][$i]));
            $match[3][$i] = trim(preg_replace("/\t/", " ", $match[3][$i]));

            if ($_SERVER['argv'][2] == "0") {
                //$buffer .= "ZEND_BEGIN_ARG_INFO_EX(arginfo_{$match[2][$i]}, 0, 0, $d)\n";
            }

            foreach ($list as $key => $l) {
                $l = preg_replace("/\/\*[\s\S]+\*\//", "", $l);
                $pl = substr_count($l, '*');
                $b = array_map(function($l){return trim($l, " *");}, explode(" ", $l));
                $type = str_replace('*', '', preg_replace('/^const /', '', substr($l, 0, strrpos($l, " "))));

                // function param has only one word => void
                if (count($b) < 2) {
                    continue;
                }

                $n = array_pop($b);
                $b = array_pop($b);
                
                if (!preg_match("/git_reflog_(entry_id_new|write)/", $match[2][$i]) &&
                    (!preg_match("/git_(packbuilder|index)_write$/", $match[2][$i])) &&
                    ($key == 0 && ($pl >= 2 || preg_match("/(cpy|out|git_strarray|(revspec|int) \*)/", $l)
                            || preg_match("/(write|create|new)/", $match[2][$i])))) {
                    $w = 1;
                    $tmp['retval'] = array(
                        "write" => $w,
                        "name" => $n,
                        "type" => $type,
                        "pointer" => $pl,
                    );
                } else {
                    $w = 0;
                }

                if (!$w) {
                    $tmp['args'][] = array(
                        "write" => $w,
                        "name" => $n,
                        "type" => $type,
                        "pointer" => $pl,
                    );

                    if ($_SERVER['argv'][2] == "0") {
                        //$buffer .= "\tZEND_ARG_INFO(0, $n)\n";
                    }
                }
            }

            if ($_SERVER['argv'][2] == "0") {
                //$buffer .= "ZEND_END_ARG_INFO()\n";
                //$buffer .= "\n";
            }

            $table[$tmp['name']] = $tmp;
        }
    }

    return $table;
}

function printFile($table, $file) {
    $buffer = "";

    if (in_array('update', $_SERVER['argv']) && is_file($file)) {
        $tmpTable = array();

        $data = file_get_contents($file);
        if (preg_match_all("/PHP_FUNCTION\((.+?)\)\s*{/s", $data, $match)) {
            foreach ($match[1] as $funcName) {
                if (isset($table[$funcName])) {
                    $tmpTable[$funcName] = $table[$funcName];
                    unset($table[$funcName]);
                }
            }
        }

        $table = array_merge($tmpTable, $table);
    }

    if ($_SERVER['argv'][2] == "1") {
        $buffer .= '#include "php_git2.h"' . "\n";
        $buffer .= '#include "php_git2_priv.h"' . "\n";
        $buffer .= '#include "' . str_replace('.c', '.h', $file) . '"' . "\n\n";
    }

    foreach ($table as $func) {
        if (functionBlacklist($func['name'])) {
            continue;
        }

        if (getPHPReturnType($func['retval']['type']) == "void" && $func['retval']['pointer'] > 0) {
            $ret = "string";
        } else {
            $ret = getPHPReturnType($func['retval']['type']);
        }

        $t = array();
        foreach ($func['args'] as $key => $a) {
            if (($key == 0 && preg_match("/(buffer|((_|^)size|len)$)/", $a['name']))
                || ($key > 0 && "{$func['args'][$key - 1]["name"]}_len" ==  $a['name'])
                || ($key > 0 && preg_match("/buffer/", $func['args'][$key - 1]["name"]) && preg_match("/len/", $a["name"]))
                || ($key > 0 && getPHPReturnType($func['args'][$key - 1]['type']) == "void"
                    && $func['args'][$key - 1]['pointer'] > 0 && preg_match("/(_|^)size$/", $a["name"]))) {
                continue;
            }

            $t[] = (isCallback($a) ? "Callable" : getPHPReturnType($a['type'])) . " \${$a['name']}";
        }

        $sig = join(", ", $t);

        $buffer .= "/* {{{ proto $ret {$func['name']}($sig)\n";
        $buffer .= " */\n";

        if ($_SERVER['argv'][2] == "0") {
            $buffer .= "PHP_FUNCTION({$func['name']});\n";
            $buffer .= "\n";
        } else {
            $buffer .= "PHP_FUNCTION({$func['name']})\n";
            $buffer .= "{\n";
            $buffer .= getDeclarations($func);

            if (count($func['args'])) {
                $buffer .= "\tif (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,\n";
                $buffer .= "\t\t" . sprintf('"%s", %s) == FAILURE) {%s', getParseStr($func), getParseStr2($func), "\n");
                $buffer .= "\t\treturn;\n";
                $buffer .= "\t}\n\n";
            }


            // --- make git_array if an argument is an array (max. 1) ---
            foreach ($func['args'] as $arg) {
                if (isOption($arg)){
                    $buffer .= "\tif ({$arg['name']} != NULL) {\n";
                    $buffer .= "\t\tphp_git2_array_to_{$arg['type']}(&_{$arg['name']}, {$arg['name']} TSRMLS_CC);\n";
                    $buffer .= "\t\tshould_free = 1;\n";
                    $buffer .= "\t}\n\n";
                } else if (isArray($arg, true)) {
                    if (preg_match("/git_signature/", $arg['type'])) {
                        $buffer .= "\tphp_git2_array_to_{$arg['type']}(_{$arg['name']}, {$arg['name']} TSRMLS_CC);\n\n";
                    } else {
                        $buffer .= "\tphp_git2_array_to_{$arg['type']}(&_{$arg['name']}, {$arg['name']} TSRMLS_CC);\n\n";
                    }
                }
            }


            foreach ($func['args'] as $key => $t) {
                if (isCallback($t)) {
                    $nameStart = explode("_",  $t['name'])[0];
                    $buffer .= "\tif (php_git2_cb_init(&{$nameStart}_cb, &{$nameStart}_fci, &{$nameStart}_fcc, {$func['args'][$key +    1]['name']} TSRMLS_CC)) {\n";
                    $buffer .= "\t\tRETURN_FALSE;\n";
                    $buffer .= "\t}\n\n";
                } else if (isResource($t)) {
                    $buffer .= "\tif ((_{$t['name']} = (php_git2_t *) zend_fetch_resource(Z_RES_P({$t['name']}), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {\n";
                    $buffer .= "\t\tRETURN_FALSE;\n";
                    $buffer .= "\t}\n\n";
                } else if (isOid($t)) {
                    $buffer .= "\tif (git_oid_fromstrn(&__{$t['name']}, {$t['name']}, {$t['name']}_len)) {\n";
                    $buffer .= "\t\tRETURN_FALSE;\n";
                    $buffer .= "\t}\n\n";
                } else if (isBuf($t)) {
                    $buffer .= "\tif (git_buf_set(&_{$t['name']}, {$t['name']}, {$t['name']}_len)) {\n";
                    $buffer .= "\t\tRETURN_FALSE;\n";
                    $buffer .= "\t}\n\n";
                }
            }


            // ------- build libgit2 function call -------
            $temp = array();

            if (hasOutValue($func)) {
                if ((isArray($func['retval'], true) && $func['retval']['pointer'] < 2)
                    || (isChar($func['retval']) && preg_match("/buffer/", $func['args'][0]['name']))) {
                    $deRef = "";
                } else {
                    $deRef = "&";
                }

                $temp[] = $deRef . $func['retval']['name'];
            }

            foreach ($func['args'] as $key => $arg) {
                if (preg_match("/void/", $arg['type'])) {
                    if  ($key > 0 && isCallback($func['args'][$key - 1])) {
                        $nameStart = explode("_", $func['args'][$key - 1]['name'])[0];
                        $temp[] = "{$nameStart}_cb";
                    } else if (preg_match("/git_transport_(local|dummy)/", $func['name']) ){
                        $temp[] = "NULL";
                    } else {
                        $temp[] = $arg['name'];
                    }
                } else if (isCallback($arg)) {
                    $temp[] = "php_git2_{$arg['type']}";
                } else if (isResource($arg)) {
                    $arr = explode("_", $arg['type'], 2);
                    $temp[] = "PHP_GIT2_V(_{$arg['name']}, " . array_pop($arr) . ")";
                } else if (isOption($arg)) {
                    $temp[] = "_{$arg['name']}";
                } else if (isArray($arg, true)) {
                    if (preg_match("/git_signature/", $arg['type'])) {
                        $temp[] = "_{$arg['name']}";
                    } else {
                        $temp[] = "&_{$arg['name']}";
                    }
                } else if (isOid($arg)) {
                    $temp[] = "&__{$arg['name']}";
                } else if (isBuf($arg)) {
                    $temp[] = "&_{$arg['name']}";
                } else {
                    $temp[] = $arg['name'];
                }
            }

            if (hasOutValue($func)) {
                $buffer .= "\t" . ($func['retval']['type'] != "void" ? "error = " : "") . "{$func['name']}(" . join(", ", $temp) . ");\n";


                // ------- build check error call -------
                $buffer .= "\n\tif (php_git2_check_error(error, \"{$func['name']}\" TSRMLS_CC)) {\n";
                $buffer .= "\t\tRETURN_FALSE;\n";
                $buffer .= "\t}\n";
            } else {
                if (isSavior($func)) {
                    $buffer .= "\tif (GIT2_SHOULD_FREE(_{$func['args'][0]['name']})) {\n";
                    $buffer .= "\t\t" . ($func['retval']['type'] != "void" ? "result = " : "") . "{$func['name']}(" . join(", ", $temp) . ");\n";
                    $buffer .= "\t\tGIT2_SHOULD_FREE(_{$func['args'][0]['name']}) = 0;\n";
                    $buffer .= "\t}\n\n";
                    $buffer .= "\tzval_ptr_dtor(&{$func['args'][0]['name']});\n";
                } else if (preg_match("/void/", $func['retval']['type']) && $func['retval']['pointer'] > 0) {
                    $buffer .= "\tbuffer = {$func['name']}(" . join(", ", $temp) . ");\n";
                } else {
                    $buffer .= "\t" . ($func['retval']['type'] != "void" ? "result = " : "") . "{$func['name']}(" . join(", ", $temp) . ");\n";
                }
            }


            // ------- build git_oid to result string -------
            if (isOid($func['retval'])) {
                if (hasOutValue($func)) {
                    $buffer .= "\n\tgit_oid_fmt(__{$func['retval']['name']}, &{$func['retval']['name']});\n";
                } else {
                    $buffer .= "\n\tgit_oid_fmt(__result, result);\n";
                }
            }


            // ------- build free option or if an argument is an array -------
            foreach ($func['args'] as $arg) {
                if (isOption($arg)) {
                    $buffer .= "\n\tif (should_free) {\n";
                    $buffer .= "\t\tphp_git2_{$arg['type']}_free(_{$arg['name']} TSRMLS_CC);\n";
                    $buffer .= "\t}\n";
                    break;
                } else if (isArray($arg, true)) {
                    if (preg_match("/git_signature/", $arg['type'])) {
                        $buffer .= "\tgit_signature_free(_{$arg['name']});\n";
                    } else if (preg_match("/git_strarray/", $arg['type'])) {
                        $buffer .= "\tgit_strarray_free(&_{$arg['name']});\n";
                    }
                }
            }


            // ------- build free callback -------
            foreach ($func['args'] as $arg) {
                if (isCallback($arg)) {
                    $nameStart = explode("_", $arg['name'])[0];
                    $buffer .= "\tphp_git2_cb_free({$nameStart}_cb);\n";
                }
            }


            // ------- make resource from out value -------
            if (isResource($func['retval'])) {
                $cleanType = strtoupper(trim(str_replace(array("git_", "const") , "", $func['retval']['type'])));
                if (hasOutValue($func)) {
                    $buffer .= "\n\tif (php_git2_make_resource(&result, PHP_GIT2_TYPE_{$cleanType}, {$func['retval']['name']}, 1 TSRMLS_CC)) {\n";
                } else {
                    $buffer .= "\n\tif (php_git2_make_resource(&__result, PHP_GIT2_TYPE_{$cleanType}, result, 1 TSRMLS_CC)) {\n";
                }
                $buffer .= "\t\tRETURN_FALSE;\n";
                $buffer .= "\t}\n";
            } else if (isArray($func['retval'], hasOutValue($func))) {
                $name = isset($func['retval']['name']) ? $func['retval']['name'] : "result";

                if (!preg_match('/git_(signature|strarray|error)/', $func['retval']['type'])) {
                    $buffer .= "\tif ($name == NULL) {\n";
                    $buffer .= "\t\tRETURN_FALSE;\n";
                    $buffer .= "\t}\n";
                }

                $name = ($func['retval']['pointer'] < 1 ? "&" : "") . $name;
                $funcPrefix = trim(str_replace("const", "", $func['retval']['type']));

                if (preg_match('/git_error/', $func['retval']['type'])) {
                    $buffer .= "\n\tphp_git2_{$funcPrefix}_to_array($name, array);\n";
                } else {
                    $buffer .= "\n\tphp_git2_{$funcPrefix}_to_array($name, array TSRMLS_CC);\n";
                }

                if (shouldBeFreed($func['retval'])) {
                    $buffer .= "\t{$funcPrefix}_free($name);\n";
                }
            }

            // -- add function relevant content insteadt of macro call build ---
            $funcRelCont = funcRelContBeforeMacro($func);

            if (strlen($funcRelCont) > 0) {
                $buffer .= $funcRelCont;
            } else {
                // ------- build return macro -------
                if ($func['retval']['type'] != "void") {
                    if (isResource($func['retval'])) {
                        if (hasOutValue($func)) {
                            $str = "return_value, GIT2_RVAL_P(result)";
                        } else {
                            $str = "return_value, GIT2_RVAL_P(__result)";
                        }
                    } else if (isArray($func['retval'], true)) {
                        $str = "array, 0, 1";
                    } else if (isOid($func['retval'])) {
                        if (hasOutValue($func)) {
                            $str = "__{$func['retval']['name']}";
                        } else {
                            $str = "__result";
                        }
                    } else if (getPHPReturnType($func['retval']['type']) == "string"
                        && (hasOutValue($func) && $func['retval']['pointer'] <= 1
                            || !hasOutValue($func) && $func['retval']['pointer'] == 0)
                        && !preg_match("/buffer/", $func['args'][0]['name'])) {
                        $str = isset($func['retval']['name']) ? "&". $func['retval']['name'] : "&result";
                    } else if (getPHPReturnType($func['retval']['type']) == "void" && $func['retval']['pointer'] > 0
                        && $func['name'] == "git_odb_object_data") {
                        $str = "buffer, git_odb_object_size(PHP_GIT2_V(_object, odb_object))";
                    } else {
                        $str = isset($func['retval']['name']) ? $func['retval']['name'] : "result";
                    }

                    if (isBuf($func['retval'])) {
                        $buffer .= "\n\tRETURN_STRINGL(git_buf_cstr($str), git_buf_len($str));\n";
                        $buffer .= "\tgit_buf_free($str);\n";
                    } else {
                        $buffer .= "\n\t" . (isBoolFunction($func) ? 'RETURN_BOOL' : getReturnMacro($func['retval']['type'])) ."($str);\n";
                    }
                }
            }

            $buffer .= "}\n";
        }

        $buffer .= "/* }}} */\n\n";
    }

    foreach (additionalFunctioToFile($file) as $name => $function) {
        $buffer .= "/* {{{ proto {$function['rettype']} $name({$function['params']})\n";
        $buffer .= " */\n";
        $buffer .= "PHP_FUNCTION($name)\n";
        $buffer .= "{\n";
        $buffer .= $function['body'];
        $buffer .= "}\n";
        $buffer .= "/* }}} */\n\n";
    }

    if (is_file($file) && in_array('update', $_SERVER['argv'])) {
        file_put_contents($file, $buffer);
    }
}


function isOption($arg) {
    return preg_match('/_opt(s|ions)$/', $arg['type']);
}

function isSavior($func) {
    return preg_match('/_free$/', $func['name']);
}

function isCallback($arg) {
    return preg_match("/_(cb|callback)$/", $arg['type']) || (isset($arg['name']) && preg_match("/_cb$/", $arg['name']));
}

function isOid($arg) {
    return preg_match('/(^|[\s\*])git_oid($|[\s\*])/', $arg['type']);
}

function isChar($arg) {
    return preg_match('/(^|[\s\*])char($|[\s\*])/', $arg['type']);
}

function isBuf($arg) {
    return preg_match('/(^|[\s\*])git_buf($|[\s\*])/', $arg['type']);
}

function isArray($arg, $firstArg = false) {
    return (!$firstArg && $arg["pointer"] == 2) || getPHPReturnType($arg['type']) == 'array';
}

function isResource($arg) {
    return getPHPReturnType($arg['type']) == 'resource';
}

function isBoolFunction($func) {
    switch ($func['name']) {
        case "git_index_set_caps":
        case "git_remote_save":
        case "git_remote_add_fetch":
        case "git_submodule_fetch_recurse_submodules":
        case "git_remote_connect":
        case "git_repository_set_namespace":
            return true;
        default:
            return preg_match("/_(is|has)($|_)/", $func['name']) && getPHPReturnType($func['rettype']) == 'long';
    }
}

function shouldBeFreed($arg) {
    $type = explode(" ", $arg['type']);
    $type = end($type);

    switch ($type) {
        case "git_blame_hunk":
        case "git_error":
        case "git_index_entry":
        case "git_transfer_progress":
        case "git_status_entry":
        case "git_revspec":
        case "git_diff_delta":
        case "git_remote_head":
            return false;
        default:
            return true;
    }
}

function hasResource($func)
{
    foreach ($func['args'] as $arg) {
        if (isResource($arg)) {
            return true;
        }
    }

    return false;
}

function hasOutValue($func) {
    return isset($func['retval']['write']);
}

function getDeclarations($func)
{
    if (hasOutValue($func)) {
        if (isResource($func['retval'])) {
            $result['php_git2_t'][] = "*result = NULL";
        } else if (isArray($func['retval'], true)) {
            $result['zval'][] = "*result";
        }

        if (isBuf($func['retval'])) {
            $result[$func['retval']['type']][] = "{$func['retval']['name']} = {0}";
        } else if (isChar($func['retval']) && preg_match("/(buffer|((_|^)size|len)$)/", $func['args'][0]['name'])) {
            $result[$func['retval']['type']][] = "{$func['retval']['name']}[GIT2_BUFFER_SIZE] = {0}";
        } else if (isArray($func['retval'], true)) {
            $result[$func['retval']['type']][] = "*{$func['retval']['name']} = NULL";
        } else {
            $result[$func['retval']['type']][] = str_repeat("*", ($func['retval']['pointer'] > 1 ? $func['retval']['pointer'] -1 : 0)) . $func['retval']['name'] .
                (getPHPReturnType($func['retval']['type']) == "long" ? " = 0" : (isOid($func['retval']) ? "" : " = NULL"));
        }

        if (isOid($func['retval'])) {
            $result[] = "\tchar __{$func['retval']['name']}[GIT2_OID_HEXSIZE] = {0};";
        }
    } else if (preg_match("/resource/", $func['retval']['type'])) {
        $result[] = "\tphp_git2_t *result = NULL;";
    } else if (!preg_match("/void/", $func['retval']['type'])) {
        $result[$func['retval']['type']][] = str_repeat("*", $func['retval']['pointer']) . "result" . ($func['retval']['pointer'] ? " = NULL" : "");

        if (isOid($func['retval'])) {
            $result[] = "\tchar __result[GIT2_OID_HEXSIZE] = {0};";
        }
    } else if ($func['retval']['pointer'] > 0) {
        $result[] = "\tconst char *buffer = NULL;";
    }

    foreach ($func['args'] as $key => $arg) {
        if (($key > 0 && "{$func['args'][$key - 1]["name"]}_len" ==  $arg['name'])) {
            continue;
        } else if (isOption($arg)) {
            $result[$arg['type']][] = "*_{$arg['name']} = NULL";
            $result['zval'][] = "*{$arg['name']} = NULL";
            $result['int'][] = "should_free = 0";
        } else if (isArray($arg)) {
            $result['zval'][] = "*{$arg['name']} = NULL";

            if (isArray($arg, true)) {
                if (preg_match("/git_signature/", $arg['type'])) {
                    $result[] = "\t{$arg['type']} *_{$arg['name']} = NULL;";
                } else {
                    $result[] = "\t{$arg['type']} _{$arg['name']} = {0};";
                }
            }
        } else if (isCallback($arg)) {
            $nameStart = explode("_",  $arg['name'])[0];
            $result['zend_fcall_info'][] = "{$nameStart}_fci = empty_fcall_info";
            $result['zend_fcall_info_cache'][] = "{$nameStart}_fcc = empty_fcall_info_cache";
            $result['php_git2_cb_t'][] = "*{$nameStart}_cb = NULL";
        } else if (isChar($arg)) {
            $result['char'][] = "*{$arg['name']} = NULL";
            $result['size_t'][] = "{$arg['name']}_len";
        } else if (getPHPReturnType($arg['type']) == "long") {
            if ($key == 0 && preg_match("/(buffer|((_|^)size|len)$)/", $arg['name'])) {
                $result[$arg['type']][] = "{$arg['name']} = GIT2_BUFFER_SIZE";
            } else {
                $result['zend_long'][] = "{$arg['name']}";
            }
        } else if (isOid($arg)) {
            $result['char'][] = "*{$arg['name']} = NULL";
            $result['size_t'][] = "{$arg['name']}_len";
            $result['git_oid'][] = "__{$arg['name']} = {0}";
        } else if (isBuf($arg)) {
            $result['char'][] = "*{$arg['name']} = NULL";
            $result['git_buf'][] = "_{$arg['name']} = {0};";
            $result['size_t'][] = "{$arg['name']}_len";
        } else if (preg_match("/git_/", $arg['type'])) {
            $result['zval'][] = "*{$arg['name']} = NULL";
            $result['php_git2_t'][] = "*_{$arg['name']} = NULL";
        } else if (preg_match("/void/", $arg['type'])) {
            if ($arg['pointer'] > 0 && count($func['args']) - 1 > $key && ($key < 1 || !isCallback($func['args'][$key - 1]))) {
                $result['char'][] = "*{$arg['name']} = NULL";
            } else {
                $result['zval'][] = "*{$arg['name']} = NULL";
            }
        }
    }

    if (isResource($func['retval'])
        || isArray($func['retval'], true)
        || getPHPReturnType($func['retval']['type']) == "string")
    {
        if (hasOutValue($func)) {
            $result[] = "\tint error = 0;";
        } else if (isArray($func['retval'])) {
            $result['zval'][] = "*array = NULL";
        } else if (getPHPReturnType($func['retval']['type']) != "string") {
            $result['php_git2_t'][] = "*__result = NULL";
        }
    }

    $result[] = "\t";
    $result[] = "";

    foreach ($result as $type => $vars) {
        if (is_array($vars)) {
            $result[$type] = "\t$type " . join(", ", $vars) . ";" ;
        }
    }

    return join("\n", $result);
}

function getParseStr($func)
{
    $hasOptional = false;
    $result = array();
    foreach ($func['args'] as $key => $arg) {
        if ($key > 0 && preg_match("/buffer/", $func['args'][$key - 1]["name"])
            && preg_match("/len/", $arg["name"])) {
            continue;
        }

        if (isOption($arg)) {
            if (!$hasOptional) {
                $result[] = '|';
            }
            $result[] = "a";
        } else if (isArray($arg)) {
            $result[] = "a";
        } else if (getPHPReturnType($arg['type']) == "string") {
            $result[] = "s";
        } else if (getPHPReturnType($arg['type']) == "long") {
            if (!(((end($result)  == 's' || $key == 0) && preg_match("/(buffer|(_|^)size$)/", $arg['name']))
                || $key == 0 && preg_match("/len$/", $arg['name'])
                || ($key > 0 && getPHPReturnType($func['args'][$key - 1]['type']) == "void" && preg_match("/len$/", $arg['name'])))) {
                $result[] = "l";
            }
        } else if (isCallback($arg)) {
            $result[] = "f";
        } else if (isResource($arg)) {
            $result[] = "r";
        } else if (preg_match("/void/", $arg['type'])) {
            if ($arg["pointer"] > 0 && count($func['args']) - 1 > $key && ($key < 1 || !isCallback($func['args'][$key - 1]))) {
                $result[] = "s";
            } else {
                $result[] = "z";
            }
        }
    }

    return join("", $result);
}

function getParseStr2($func)
{
    $result = array();
    foreach ($func['args'] as $key => $arg) {
        if ($key == 0 && preg_match("/(buffer|((_|^)size|len)$)/", $arg['name'])
            || ($key > 0 && "{$func['args'][$key - 1]["name"]}_len" ==  $arg['name'])) {
            continue;
        }

        if (isCallback($arg)) {
            $nameStart = explode("_",  $arg['name'])[0];

            $result[] = "&{$nameStart}_fci";
            $result[] = "&{$nameStart}_fcc";
            continue;
        }

        $result[] = "&{$arg['name']}";

        if (getPHPReturnType($arg['type']) == "string" && !isArray($arg)) {
            $result[] = "&{$arg['name']}_len";
        }
    }

    return join(", ", $result);
}

function getPHPReturnType($cType)
{
    $arg = array('type' => $cType);

    if (isCallback($arg)) {
        return "Callable";
    }

    if (isOption($arg)) {
        return "array";
    }


    if (preg_match('/_t$/', $cType)) {
        /*
         * git_attr_t
         * git_branch_t
         * git_delta_t
         * git_filemode_t
         * git_off_t
         * git_remote_autotag_option_t
         * git_reset_t
         * git_submodule_ignore_t
         * git_submodule_update_t
         * git_time_t
         * size_t
         * uint32_t
         */
        return "long";
    }

    if (isChar($arg) || isOid($arg) || isBuf($arg)) {
        return "string";
    }

    $cType = explode(" ", $cType);
    $cType = end($cType);

    switch($cType) {
        case "int":
        case "unsigned":
        case "git_otype":
        case "git_direction":
        case "git_treewalk_mode":
            return "long";
        case "void":
            return "void";
        case "git_error":
        case "git_signature":
        case "git_strarray" :
        case "git_index_entry":
        case "git_status_entry":
        case "git_blame_hunk":
        case "git_transfer_progress":
        case "git_remote_callbacks":
        case "git_revspec":
        case "git_diff_delta":
        case "git_remote_head":
            return "array";
        default:
            return "resource";
    }
}

function getReturnMacro($cType)
{
    $type = getPHPReturnType($cType);

    switch($type) {
        case "long":
            return "RETURN_LONG";
        case "string":
            return "RETURN_STRING";
        case "array":
            return "RETURN_ZVAL";
        case "void":
            return "RETURN_STRINGL";
        default:
            return "ZVAL_RESOURCE";
    }
}

function funcRelContBeforeMacro($func)
{
    $buffer = "";

    if ($func["name"] == "git_blob_rawcontent") {
        $buffer .= "\n\tif (buffer == NULL){\n";
        $buffer .= "\t\tRETURN_FALSE;\n";
        $buffer .= "\t}\n\n";
        $buffer .= "\tsize = git_blob_rawsize(PHP_GIT2_V({$func["args"][0]["name"]}, blob));\n";
        $buffer .= "\tRETURN_STRINGL(buffer, size);\n";
    }

    return $buffer;
}

function functionBlacklist($funcName)
{
    $blackList = array(
        "git_signature_free" => true,
        "git_signature_dup" => true,
        "git_commit_create_v" => true,
        "git_commit_create_from_oids" => true,
        "git_reflog_entry__free" => true,
        "git_reflog_entry__alloc" => true,
        "git_index_name_entrycount" => true,
        "git_index_name_get_byindex" => true,
        "git_index_name_add" => true,
        "git_index_name_clear" => true,
        "git_index_reuc_entrycount" => true,
        "git_index_reuc_find" => true,
        "git_index_reuc_get_bypath" => true,
        "git_index_reuc_get_byindex" => true,
        "git_index_reuc_add" => true,
        "git_index_reuc_remove" => true,
        "git_index_reuc_clear" => true,
        "git_reference__alloc" => true,
        "git_reference__alloc_symbolic" => true,
        "git_repository__cleanup" => true,
        "git_repository_reinit_filesystem" => true,
        "git_repository_set_config" => true,
        "git_repository_set_odb" => true,
        "git_repository_set_refdb" => true,
        "git_repository_set_index" => true,
        "git_tree_entry_to_object" => true,
        "git_tree_lookup_prefix" => true,
        "git_odb_backend_one_pack" => true,
        "git_odb_backend_pack" => true,
        "git_odb_backend_loose" => true,
    );

    return isset($blackList[$funcName]) && $blackList[$funcName];
}

function additionalFunctioToFile($fileName)
{
    $functions = array();

    if ($fileName == "blame.h" || $fileName == "blame.c") {
        $func = "\tzval *result;\n";
        $func .= "\tgit_blame_options options = GIT_BLAME_OPTIONS_INIT;\n\n";
        $func .= "\tphp_git2_git_blame_options_to_array(&options, result TSRMLS_CC);\n";
        $func .= "\tRETURN_ZVAL(result, 0, 1);\n";

        $functions = array(
            'git_blame_options_new' => array(
                'body' => $func,
                'params' => "",
                'rettype' => "array"
            )
        );
    } else if ($fileName == "repository.h" || $fileName == "repository.c") {
        $func = "\tzval *result;\n";
        $func .= "\tgit_repository_init_options options = GIT_REPOSITORY_INIT_OPTIONS_INIT;\n\n";
        $func .= "\tphp_git2_git_repository_init_options_to_array(&options, result TSRMLS_CC);\n";
        $func .= "\tRETURN_ZVAL(result, 0, 1);\n";

        $functions = array(
            'git_repository_init_options_new' => array(
                'body' => $func,
                'params' => "",
                'rettype' => "array"
            )
        );
    } else if ($fileName == "status.h" || $fileName == "status.c") {
        $func = "\tzval *result;\n";
        $func .= "\tgit_status_options options = GIT_STATUS_OPTIONS_INIT;\n\n";
        $func .= "\tphp_git2_git_status_options_to_array(&options, result TSRMLS_CC);\n";
        $func .= "\tRETURN_ZVAL(result, 0, 1);\n";

        $functions = array(
            'git_status_options_new' => array(
                'body' => $func,
                'params' => "",
                'rettype' => "array"
            )
        );
    } else if ($fileName == "filter.h" || $fileName == "filter.c") {
        $func = "\tgit_filter *filter = NULL;\n";
        $func .= "\tphp_git2_t *result = NULL;\n";
        $func .= "\tphp_git2_filter *_filter = NULL;\n";
        $func .= "\tzval *mixed = NULL, *tmp = NULL;\n";
        $func .= "\tzend_fcall_info initialize_fci, shutdown_fci, check_fci, apply_fci, cleanup_fci;\n";
        $func .= "\tzend_fcall_info_cache initialize_fcc, shutdown_fcc, check_fcc, apply_fcc, cleanup_fcc;\n\n";
        $func .= "\tif (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,\n";
        $func .= "\t\t\"a\", &mixed) == FAILURE) {\n";
        $func .= "\t\treturn;\n";
        $func .= "\t}\n\n";
        $func .= "\t_filter = (php_git2_filter*)ecalloc(1, sizeof(php_git2_filter));\n";
        $func .= "\tfilter = (git_filter*)&_filter->super;\n";
        $func .= "\tfilter->version = GIT_FILTER_VERSION;\n";
        $func .= "\tfilter->attributes = php_git2_read_arrval_string(mixed, ZEND_STRS(\"attributes\") TSRMLS_CC);\n\n";
        $func .= "\tif (tmp = php_git2_read_arrval(mixed, ZEND_STRS(\"initialize\") TSRMLS_CC)) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &initialize_fci, &initialize_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\tif (tmp = php_git2_read_arrval(mixed, ZEND_STRS(\"shutdown\") TSRMLS_CC)) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &shutdown_fci, &shutdown_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\tif (tmp = php_git2_read_arrval(mixed, ZEND_STRS(\"check\") TSRMLS_CC)) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &check_fci, &check_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\tif (tmp = php_git2_read_arrval(mixed, ZEND_STRS(\"apply\") TSRMLS_CC)) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &apply_fci, &apply_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\tif (tmp = php_git2_read_arrval(mixed, ZEND_STRS(\"cleanup\") TSRMLS_CC)) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &cleanup_fci, &cleanup_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\tfilter->initialize = php_git2_git_filter_init_fn;\n";
        $func .= "\tfilter->shutdown = php_git2_git_filter_shutdown_fn;\n";
        $func .= "\tfilter->check = php_git2_git_filter_check_fn;\n";
        $func .= "\tfilter->apply = php_git2_git_filter_apply_fn;\n";
        $func .= "\tfilter->cleanup = php_git2_git_filter_cleanup_fn;\n\n";
        $func .= "\tZ_ADDREF_P(mixed);\n";
        $func .= "\tphp_git2_multi_cb_init(&_filter->multi, mixed TSRMLS_CC, 5,\n";
        $func .= "\t\t&initialize_fci, &initialize_fcc,\n";
        $func .= "\t\t&shutdown_fci, &shutdown_fcc,\n";
        $func .= "\t\t&check_fci, &check_fcc,\n";
        $func .= "\t\t&apply_fci, &apply_fcc,\n";
        $func .= "\t\t&cleanup_fci, &cleanup_fcc\n";
        $func .= "\t);\n\n";
        $func .= "\tif (php_git2_make_resource(&result, PHP_GIT2_TYPE_FILTER, filter, 1 TSRMLS_CC)) {\n";
        $func .= "\t\tRETURN_FALSE;\n";
        $func .= "\t}\n\n";
        $func .= "\tZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));\n";


        $functions = array('git_filter_new' => array('body' => $func, 'params' => "", 'rettype' => "array"));
    } else if ($fileName == "odb.h" || $fileName == "odb.c") {
        $func = "\tphp_git2_odb_backend *backend;\n";
        $func .= "\tphp_git2_t *result;\n";
        $func .= "\tzval *callbacks, *tmp;\n";
        $func .= "\tzend_fcall_info read_fci, write_fci, read_prefix_fci, read_header_fci, writestream_fci,\n";
        $func .= "\t\texists_fci, foreach_fci, free_fci, refresh_fci;\n";
        $func .= "\tzend_fcall_info_cache read_fcc, write_fcc, read_prefix_fcc, read_header_fcc, writestream_fcc,\n";
        $func .= "\t\texists_fcc, foreach_fcc, free_fcc, refresh_fcc;\n\n";
        $func .= "\tif (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,\n";
        $func .= "\t\t\"a\", &callbacks) == FAILURE) {\n";
        $func .= "\t\treturn;\n";
        $func .= "\t}\n\n";
        $func .= "\tbackend = ecalloc(1, sizeof(php_git2_odb_backend));\n";
        $func .= "\tbackend->parent.version = GIT_ODB_BACKEND_VERSION;\n";
        $func .= "\tbackend->parent.read = &php_git2_odb_backend_read;\n";
        $func .= "\tbackend->parent.write = &php_git2_odb_backend_write;\n";
        $func .= "\tbackend->parent.read_prefix = &php_git2_odb_backend_read_prefix;\n";
        $func .= "\tbackend->parent.read_header = &php_git2_odb_backend_read_header;\n";
        $func .= "\t//backend->parent.writestream = &php_git2_odb_backend_writestream;\n";
        $func .= "\tbackend->parent.exists = &php_git2_odb_backend_exists;\n";
        $func .= "\tbackend->parent.foreach = &php_git2_odb_backend_foreach;\n";
        $func .= "\tbackend->parent.free = &php_git2_odb_backend_free;\n";
        $func .= "\tbackend->parent.refresh = &php_git2_odb_refresh;\n\n";
        $func .= "\ttmp = php_git2_read_arrval(callbacks, ZEND_STRS(\"read\") TSRMLS_CC);\n";
        $func .= "\tif (tmp) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &read_fci, &read_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\ttmp = php_git2_read_arrval(callbacks, ZEND_STRS(\"write\") TSRMLS_CC);\n";
        $func .= "\tif (tmp) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &write_fci, &write_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\ttmp = php_git2_read_arrval(callbacks, ZEND_STRS(\"read_header\") TSRMLS_CC);\n";
        $func .= "\tif (tmp) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &read_header_fci, &read_header_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\ttmp = php_git2_read_arrval(callbacks, ZEND_STRS(\"read_prefix\") TSRMLS_CC);\n";
        $func .= "\tif (tmp) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &read_prefix_fci, &read_prefix_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\ttmp = php_git2_read_arrval(callbacks, ZEND_STRS(\"exists\") TSRMLS_CC);\n";
        $func .= "\tif (tmp) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &exists_fci, &exists_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\ttmp = php_git2_read_arrval(callbacks, ZEND_STRS(\"foreach\") TSRMLS_CC);\n";
        $func .= "\tif (tmp) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &foreach_fci, &foreach_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\ttmp = php_git2_read_arrval(callbacks, ZEND_STRS(\"free\") TSRMLS_CC);\n";
        $func .= "\tif (tmp) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &free_fci, &free_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\ttmp = php_git2_read_arrval(callbacks, ZEND_STRS(\"refresh\") TSRMLS_CC);\n";
        $func .= "\tif (tmp) {\n";
        $func .= "\t\tphp_git2_fcall_info_wrapper2(tmp, &refresh_fci, &refresh_fcc TSRMLS_CC);\n";
        $func .= "\t}\n\n";
        $func .= "\tZ_ADDREF_P(callbacks);\n";
        $func .= "\tphp_git2_multi_cb_init(&backend->multi, callbacks TSRMLS_CC, 9,\n";
        $func .= "\t\t&read_fci, &read_fcc,\n";
        $func .= "\t\t&write_fci, &write_fcc,\n";
        $func .= "\t\t&read_prefix_fci, &read_prefix_fcc,\n";
        $func .= "\t\t&read_header_fci, &read_header_fcc,\n";
        $func .= "\t\t&writestream_fci, &writestream_fcc,\n";
        $func .= "\t\t&exists_fci, &exists_fcc,\n";
        $func .= "\t\t&foreach_fci, &foreach_fcc,\n";
        $func .= "\t\t&free_fci, &free_fcc,\n";
        $func .= "\t\t&refresh_fci, &refresh_fcc\n";
        $func .= "\t);\n\n";
        $func .= "\tif (php_git2_make_resource(&result, PHP_GIT2_TYPE_ODB_BACKEND, backend, 1 TSRMLS_CC)) {\n";
        $func .= "\t\tRETURN_FALSE;\n";
        $func .= "\t}\n\n";
        $func .= "\tZVAL_RESOURCE(return_value, GIT2_RVAL_P(result));\n";

        $functions = array('git_odb_backend_new' => array('body' => $func, 'params' => "", 'rettype' => "array"));
    }

    return $functions;
}