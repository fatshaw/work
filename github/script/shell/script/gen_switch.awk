BEGIN {
    x = "debug_enable";	str[x] = "-"; mac[x] = "-"; xstr[x] = "-";
    x = "amer";		str[x] = "opt_tab"; mac[x] = "THW_PPC"; xstr[x] = "-";
    x = "sparc";	str[x] = "opt_tab"; mac[x] = "THW_SPARC"; xstr[x] = "-";
    x = "s370";		str[x] = "opt_tab"; mac[x] = "THW_370"; xstr[x] = "-";
    x = "ia64";		str[x] = "opt_tab"; mac[x] = "THW_IA64"; xstr[x] = "-";
    x = "cell_spu";	str[x] = "opt_tab"; mac[x] = "THW_CELLSPU"; xstr[x] = "-";
    x = "bgq";          str[x] = "opt_tab"; mac[x] = "THW_PPCBGQ"; xstr[x] = "-";
    x = "aix";		str[x] = "opt_tab"; mac[x] = "TOS_AIX"; xstr[x] = "-";
    x = "ntos";		str[x] = "opt_tab"; mac[x] = "TOS_WINNT"; xstr[x] = "-";
    x = "wpos";		str[x] = "opt_tab"; mac[x] = "TOS_WPOS"; xstr[x] = "-";
    x = "system7";	str[x] = "opt_tab"; mac[x] = "TOS_SYSTEM7"; xstr[x] = "-";
    x = "solaris";	str[x] = "opt_tab"; mac[x] = "TOS_SOLARIS"; xstr[x] = "-";
    x = "sunos";	str[x] = "opt_tab"; mac[x] = "TOS_SUNOS"; xstr[x] = "-";
    x = "mvs";		str[x] = "opt_tab"; mac[x] = "TOS_MVS"; xstr[x] = "-";
    x = "hpux";		str[x] = "opt_tab"; mac[x] = "TOS_HPUX"; xstr[x] = "-";
    x = "linux_os";	str[x] = "opt_tab"; mac[x] = "TOS_LINUX"; xstr[x] = "-";
    x = "mac_os";	str[x] = "opt_tab"; mac[x] = "TOS_MACOS"; xstr[x] = "-";
    x = "c_lang";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "lan_lang";
    x = "cobol";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "lan_lang";
    x = "cplusplus";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "lan_lang";
    x = "pl9";		str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "lan_lang";
    x = "fortran";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "lan_lang";
    x = "pl1_lang";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "lan_lang";
    x = "ada";		str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "lan_lang";
    x = "plx_lang";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "lan_lang";
    x = "pascal";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "lan_lang";
    x = "rpg";		str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "lan_lang";
    x = "java_lang";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "lan_lang";
    x = "opencl_lang";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "lan_lang";
    x = "xcoff_object";	str[x] = "obe_global"; mac[x] = "SUPPORT_XCOFF"; xstr[x] = "-";
    x = "elf_object";	str[x] = "obe_global"; mac[x] = "SUPPORT_ELF"; xstr[x] = "-";
    x = "die_support";	str[x] = "-"; mac[x] = "SUPPORT_DIE"; xstr[x] = "-";
    x = "sparc_coff_object";str[x] = "obe_global"; mac[x] = "SUPPORT_SUN_COFF"; xstr[x] = "-";
    x = "text_object";	str[x] = "obe_global"; mac[x] = "SUPPORT_OBJ370"; xstr[x] = "-";
    x = "hp_aout_object"; str[x] = "obe_global"; mac[x] = "SUPPORT_HP_DOT_O"; xstr[x] = "-";
    x = "mcoff_object";	str[x] = "obe_global"; mac[x] = "SUPPORT_MCOFF"; xstr[x] = "-";
    x = "macho_object";	str[x] = "obe_global"; mac[x] = "-"; xstr[x] = "-";
    x = "toc_abi";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "obe_bits_misc3";
    x = "poweropen_abi"; str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "obe_bits_misc3";
    x = "got_abi";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "obe_bits_misc3";
    x = "linppc_abi";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "obe_bits_misc3";
    x = "ntppc_abi";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "obe_bits_misc3";
    x = "ntia64_abi";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "obe_bits_misc3";
    x = "aixia64_abi";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "obe_bits_misc3";
    x = "little_endian";str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "obe_bits_misc3";
    x = "fp_emulate";	str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "obe_bits_misc";
    x = "hummer";	str[x] = "-"; mac[x] = "THW_HUMMER"; xstr[x] = "-";
    x = "ieee";		str[x] = "gl_obe.machine_characteristics"; mac[x] = "-"; xstr[x] = "-";
    x = "machine_fpregs"; str[x] = "obe_bits"; mac[x] = "-"; xstr[x] = "-";
    on_off["die_support"] = "";
}
/[Dd][Cc][Ll][^!]*[Ll][Ii][Tt][Ee][Rr][Aa][Ll][Ll][Yy][ 	]*\([\"'\'' ]*[0-9]/ {
    match($0, /[Ll][Ii][Tt][Ee][Rr][Aa][Ll][Ll][Yy][ 	]*\([\"'\'' ]*/);
    literal = substr($0, RSTART+RLENGTH, 1);
    match($0, /[Dd][Cc][Ll] */);
    nameetc = substr($0, RSTART+RLENGTH);
    name = substr(nameetc, 1, index(nameetc," ")-1);
    if ($1 == "!") {
	on_off[name] = "!";
    } else {
	on_off[name] = literal;
	if (name == "elf_object") {
	    on_off["die_support"] = literal;
	}
    }
}
END {
    for (p in mac) {
	printf("\n/* %s */\n", p);
	if (p == "little_endian") {
	    fixup = 1;
	} else {
	    fixup = 0;
	}
	if (on_off[p] == "!") {
	    if (str[p] != "-") {
		aaa = toupper(p);
		printf("#undef %s\n", aaa);
		if (xstr[p] != "-") {
		    printf("    #define %s %s.%s.%s\n", aaa, str[p], xstr[p], p);
		    if (fixup) {
			printf("    #define X%s %s.%s.%s\n", aaa, str[p], xstr[p], p);
		    }
		} else {
		    printf("#define %s %s.%s\n", aaa, str[p], p);
		    if (fixup) {
			printf("#define X%s %s.%s\n", aaa, str[p], p);
		    }
	       }
	    }
	} else if ((on_off[p] == "0") || (on_off[p] == "")) {
	    printf("#undef %s\n#define %s 0\n", toupper(p), toupper(p));
	    if (fixup) {
		printf("#define X%s 0\n", toupper(p));
	    }
	} else {
	    printf("#undef %s\n#define %s 1\n", toupper(p), toupper(p));
	    if (fixup) {
		printf("#define X%s 1\n", toupper(p));
	    }
	}
       
	if (mac[p] == "-") {
	} else if ((on_off[p] == "0") || (on_off[p] == "")) {
	    printf("#undef %s\n", mac[p]);
	} else {
	    printf("#ifndef %s\n#define %s\n#endif\n", mac[p], mac[p]);
	}
    }
    if (length(on_off["max_opt"]) > 0) {
	printf("#undef MAX_OPT\n#define MAX_OPT %s\n", on_off["max_opt"]);
    }
    printf("\n");
}
