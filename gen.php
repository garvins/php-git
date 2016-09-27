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
        if (!preg_match('/\w.h$/' , $file)) {
           continue;
        }

        $newFile = preg_replace('/h$/', ($_SERVER['argv'][2] == 0 ? 'h' : 'c'), $file);

        echo "Start parsing File $file .........";
        $data = parseFile($_SERVER['argv'][1] . '/' . $file);

        if (count($data)) {
            echo " " . count($data) ." functions found.\n";
            printFile($data, $newFile);
        }
        else {
            echo " nothing found.\n";
        }
    }
} else if (is_file($_SERVER['argv'][1])) {
    $newFile = array_pop(explode('/', $_SERVER['argv'][1]));

    printFile(parseFile($_SERVER['argv'][1]), $newFile);
}

function cleanArgs () {

}

function parseFile($path){
    $data = file_get_contents($path);

    $table = array();
    if (preg_match_all("/GIT_EXTERN\((.+?)\)\s*([a-zA-Z0-9_-]+)\((.+?)\);/s", $data, $match)) {

        for ($i = 0; $i < count($match[0]); $i++) {
            $tmp = array(
                "args" => array(),
            );

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

            $o = 0;
            foreach ($list as $l) {
                $pl = substr_count($l, '*');
                $b = array_map(function($l){return trim($l, " *");}, explode(" ", $l));
                $type = str_replace('*', '', preg_replace('/^const /', '', substr($l, 0, strrpos($l, " "))));

                $n = array_pop($b);
                $b = array_pop($b);

                if ($o == 0 && (preg_match("/(\\*\\*|out|git_strarray)/", $l) || preg_match("/(write|create|new)/", $match[2][$i]))) {
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

                $o++;
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
        $ret = getPHPReturnType($func['retval']['type']);

        $t = array();
        foreach ($func['args'] as $a) {
            $t[] = getPHPReturnType($a['type']) . " \${$a['name']}";
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

            $buffer .= "\tif (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,\n";
            $buffer .= "\t\t" . sprintf('"%s", %s) == FAILURE) {%s', getParseStr($func), getParseStr2($func), "\n");
            $buffer .= "\t\treturn;\n";
            $buffer .= "\t}\n\n";


            // --- make git_array if an argument is an array (max. 1) ---
            foreach ($func['args'] as $arg) {
                if (isArray($arg, true)) {
                    $buffer .= "\tphp_git2_array_to_" . str_replace('git_', '', $arg['type']) . "(&out, {$arg['name']} TSRMLS_CC);\n\n";
                    break;
                }
            }


            foreach ($func['args'] as $t) {
                if (isCallback($t)) {
                    $buffer .= "\tif (php_git2_cb_init(&cb, &fci, &fcc, payload TSRMLS_CC)) {\n";
                    $buffer .= "\t\tRETURN_FALSE;\n";
                    $buffer .= "\t}\n\n";
                } else if (isResource($t)) {
                    $buffer .= "\tif ((_{$t['name']} = (php_git2_t *) zend_fetch_resource(Z_RES_P({$t['name']}), PHP_GIT2_RESOURCE_NAME, git2_resource_handle)) == NULL) {\n";
                    $buffer .= "\t\tRETURN_FALSE;\n";
                    $buffer .= "\t}\n\n";
                }
            }


            // ------- build libgit2 function call -------
            $temp = array();

            if (isset($func['retval']['write'])) {
                $temp[] = "&" . $func['retval']['name'];
            }

            foreach ($func['args'] as $arg) {
                if (preg_match("/void/", $arg['type'])) {
                    $temp[] = "cb";
                } else if (isCallback($arg)) {
                    $temp[] = "php_git2_{$arg['type']}";
                } else if (isResource($arg)) {
                    $arr = explode("_", $arg['type']);
                    $temp[] = "PHP_GIT2_V(_{$arg['name']}, " . array_pop($arr) . ")";
                } else if (isArray($arg, true)) {
                    $temp[] = "&out";
                } else {
                    $temp[] = $arg['name'];
                }


            }

            if (hasOutValue($func)) {
                $buffer .= "\t" . ($func['retval']['type'] != "void" ? "error = " : "") . "{$func['name']}(" . join(", ", $temp) . ");\n";


                // ------- build array to result -------
                if (isArray($func['retval'], true)) {
                    $buffer .= "\tphp_git2_" . str_replace("git_", "", $func['retval']['type']) . "_to_array(&{$func['retval']['name']}, &result TSRMLS_CC);\n";
                    $buffer .= "\t{$func['retval']['type']}_free(&{$func['retval']['name']});\n";
                }


                // ------- build check error call -------
                if (hasOutValue($func)) {
                    $buffer .= "\n\tif (php_git2_check_error(error, \"{$func['name']}\" TSRMLS_CC)) {\n";
                    $buffer .= "\t\tRETURN_FALSE;\n";
                    $buffer .= "\t}\n";
                }
            } else {
                $buffer .= "\t" . ($func['retval']['type'] != "void" ? "result = " : "") . "{$func['name']}(" . join(", ", $temp) . ");\n";
            }


            // --- free if an argument is an array (max. 1) ---
            foreach ($func['args'] as $arg) {
                if (isArray($arg, true)) {
                    $buffer .= "\tphp_git2_" . str_replace('git_', '', $arg['type']) ."_free(&out);\n";
                    break;
                }
            }


            // ------- build free callback -------
            foreach ($func['args'] as $arg) {
                if (isCallback($arg)) {
                    $buffer .= "\tphp_git2_cb_free(cb);\n";
                    break;
                }
            }


            // ------- make resource from out value -------
            if (isResource($func['retval'])) {
                if (hasOutValue($func)) {
                    $buffer .= "\n\tif (php_git2_make_resource(&result, PHP_GIT2_TYPE_" .
                        strtoupper(str_replace("git_", "", $func['retval']['type'])) . ", {$func['retval']['name']}, 1 TSRMLS_CC)) {\n";
                } else {
                    $buffer .= "\n\tif (php_git2_make_resource(&__result, PHP_GIT2_TYPE_" .
                        strtoupper(str_replace("git_", "", $func['retval']['type'])) . ", result, 1 TSRMLS_CC)) {\n";
                }
                $buffer .= "\t\tRETURN_FALSE;\n";
                $buffer .= "\t}\n";
            }


            // ------- build return macro -------
            if ($func['retval']['type'] != "void") {
                if (isResource($func['retval'])) {
                    if (hasOutValue($func)) {
                        $str = "return_value, GIT2_RVAL_P(result)";
                    } else {
                        $str = "return_value, GIT2_RVAL_P(__result)";
                    }
                } else if (isArray($func['retval'], true)) {
                    $str = "result, 0, 1";
                } else {
                    $str = isset($func['retval']['name']) ? $func['retval']['name'] : "result";
                }

                $buffer .= "\n\t" . (isBoolFunction($func) ? 'RETURN_BOOL' : getReturnMacro($func['retval']['type'])) ."($str);\n";
            }

            $buffer .= "}\n";
        }

        $buffer .= "/* }}} */\n\n";
    }

    file_put_contents($file, $buffer);
}

function isCallback($arg) {
    return preg_match("/_cb$/", $arg['type']);
}

function isArray($arg, $firstArg = false) {
    return ($arg["pointer"] == 2 && !$firstArg) || getPHPReturnType($arg['type']) == 'array';
}

function isResource($arg) {
    return getPHPReturnType($arg['type']) == 'resource';
}

function isBoolFunction($func) {
    switch ($func['name']) {
        case "git_remote_set_transport":
        case "git_remote_set_fetch_refspecs":
        case "git_remote_set_push_refspecs":
            return false;
        case "git_remote_save":
        case "git_remote_add_fetch":
            return true;
        default:
            return preg_match("/_(is|set)($|_)/", $func['name']) && getPHPReturnType($func['rettype']) == 'long';
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

        $result[$func['retval']['type']][] = str_repeat("*", $func['retval']['pointer'] - 1) . "{$func['retval']['name']}" .
            (getPHPReturnType($func['retval']['type']) == "long" ? " = 0" : (isArray($func['retval'], true) ? " = {0}" : " = NULL"));
    } else if (preg_match("/resource/", $func['retval']['type'])) {
        $result[] = "\tphp_git2_t *result = NULL;";
    } else if (!preg_match("/void/", $func['retval']['type'])) {
        $result[] = "\t{$func['retval']['type']} " . str_repeat("*", $func['retval']['pointer']) . "result" .
            ($func['retval']['pointer'] ? " = NULL" : "") .";";
    }

    foreach ($func['args'] as $arg) {
        if (isArray($arg)) {
            $result['zval'][] = "*{$arg['name']} = NULL";

            if (isArray($arg, true)) {
                $result[] = "\t{$arg['type']} out = {0};";
            }
        } else if (isCallback($arg)) {
            $result['zend_fcall_info'][] = "fci = empty_fcall_info";
            $result['zend_fcall_info_cache'][] = "fcc = empty_fcall_info_cache";
            $result['php_git2_cb_t'][] = "*cb = NULL";
        } else if (preg_match("/char/", $arg['type'])) {
            $result['char'][] = "*{$arg['name']} = NULL";
            $result['size_t'][] = "{$arg['name']}_len";
        } else if (getPHPReturnType($arg['type']) == "long") {
            $result['zend_long'][] = "{$arg['name']}";
        } else if (preg_match("/git_oid/", $arg['type'])) {
            $result['char'][] = "*{$arg['name']} = NULL";
            $result['size_t'][] = "{$arg['name']}_len";
        } else if (preg_match("/git_/", $arg['type'])) {
            $result['zval'][] = "*{$arg['name']} = NULL";
            $result['php_git2_t'][] = "*_{$arg['name']} = NULL";
        } else if (preg_match("/void/", $arg['type'])) {
            $result['zval'][] = "*{$arg['name']} = NULL";
        }
    }

    if (isResource($func['retval']) || isArray($func['retval'], true))
    {
        if (hasOutValue($func)) {
            $result[] = "\tint error = 0;";
        } else {
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
    $result = array();
    foreach ($func['args'] as $arg) {
        if (isArray($arg)) {
            $result[] = "a";
        } else if (preg_match("/(char|git_oid)/", $arg['type'])) {
            $result[] = "s";
        } else if (getPHPReturnType($arg['type']) == "long") {
            $result[] = "l";
        } else if (isCallback($arg)) {
            $result[] = "f";
        } else if (isResource($arg)) {
            $result[] = "r";
        } else if (preg_match("/void/", $arg['type'])) {
            $result[] = "z";
        }
    }

    return join("", $result);
}

function getParseStr2($func)
{
    $result = array();
    foreach ($func['args'] as $arg) {
        if (isCallback($arg)) {
            $result[] = "&fci";
            $result[] = "&fcc";
            continue;
        }

        $result[] = "&{$arg['name']}";
        if (isArray($arg)) {
        } else if (preg_match("/char/", $arg['type'])) {
            $result[] = "&{$arg['name']}_len";
        } else if (preg_match("/git_oid/", $arg['type'])) {
            $result[] = "&{$arg['name']}_len";
        }
    }

    return join(", ", $result);
}

function getPHPReturnType($cType)
{
    if (isCallback(array('type' => $cType))) {
        return "Callable";
    }

    switch($cType) {
        case "int":
        case "size_t":
        case "uint32_t":
        case "git_attr_t":
        case "git_remote_autotag_option_t":
            return "long";
        case "void":
            return "void";
        case "git_oid":
        case "char":
        case "const char":
            return "string";
        case "git_signature":
        case "git_strarray" :
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
            return null;
        default:
            return "ZVAL_RESOURCE";
    }
}
