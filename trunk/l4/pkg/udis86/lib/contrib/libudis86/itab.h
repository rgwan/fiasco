#ifndef UD_ITAB_H
#define UD_ITAB_H

/* itab.h -- generated by udis86:scripts/ud_itab.py, do no edit */

/* ud_table_type -- lookup table types (see decode.c) */
enum ud_table_type {
    UD_TAB__OPC_TABLE,
    UD_TAB__OPC_VENDOR,
    UD_TAB__OPC_REG,
    UD_TAB__OPC_SSE,
    UD_TAB__OPC_MOD,
    UD_TAB__OPC_RM,
    UD_TAB__OPC_VEX_P,
    UD_TAB__OPC_VEX_M,
    UD_TAB__OPC_OSIZE,
    UD_TAB__OPC_MODE,
    UD_TAB__OPC_X87,
    UD_TAB__OPC_3DNOW,
    UD_TAB__OPC_ASIZE
};

/* ud_mnemonic -- mnemonic constants */
enum ud_mnemonic_code {
    UD_Iinvalid,
    UD_I3dnow,
    UD_Inone,
    UD_Idb,
    UD_Ipause,
    UD_Iaaa,
    UD_Iaad,
    UD_Iaam,
    UD_Iaas,
    UD_Iadc,
    UD_Iadd,
    UD_Iaddpd,
    UD_Iaddps,
    UD_Iaddsd,
    UD_Iaddss,
    UD_Iand,
    UD_Iandpd,
    UD_Iandps,
    UD_Iandnpd,
    UD_Iandnps,
    UD_Iarpl,
    UD_Imovsxd,
    UD_Ibound,
    UD_Ibsf,
    UD_Ibsr,
    UD_Ibswap,
    UD_Ibt,
    UD_Ibtc,
    UD_Ibtr,
    UD_Ibts,
    UD_Icall,
    UD_Icbw,
    UD_Icwde,
    UD_Icdqe,
    UD_Iclc,
    UD_Icld,
    UD_Iclflush,
    UD_Iclgi,
    UD_Icli,
    UD_Iclts,
    UD_Icmc,
    UD_Icmovo,
    UD_Icmovno,
    UD_Icmovb,
    UD_Icmovae,
    UD_Icmovz,
    UD_Icmovnz,
    UD_Icmovbe,
    UD_Icmova,
    UD_Icmovs,
    UD_Icmovns,
    UD_Icmovp,
    UD_Icmovnp,
    UD_Icmovl,
    UD_Icmovge,
    UD_Icmovle,
    UD_Icmovg,
    UD_Icmp,
    UD_Icmppd,
    UD_Icmpps,
    UD_Icmpsb,
    UD_Icmpsw,
    UD_Icmpsd,
    UD_Icmpsq,
    UD_Icmpss,
    UD_Icmpxchg,
    UD_Icmpxchg8b,
    UD_Icmpxchg16b,
    UD_Icomisd,
    UD_Icomiss,
    UD_Icpuid,
    UD_Icvtdq2pd,
    UD_Icvtdq2ps,
    UD_Icvtpd2dq,
    UD_Icvtpd2pi,
    UD_Icvtpd2ps,
    UD_Icvtpi2ps,
    UD_Icvtpi2pd,
    UD_Icvtps2dq,
    UD_Icvtps2pi,
    UD_Icvtps2pd,
    UD_Icvtsd2si,
    UD_Icvtsd2ss,
    UD_Icvtsi2ss,
    UD_Icvtss2si,
    UD_Icvtss2sd,
    UD_Icvttpd2pi,
    UD_Icvttpd2dq,
    UD_Icvttps2dq,
    UD_Icvttps2pi,
    UD_Icvttsd2si,
    UD_Icvtsi2sd,
    UD_Icvttss2si,
    UD_Icwd,
    UD_Icdq,
    UD_Icqo,
    UD_Idaa,
    UD_Idas,
    UD_Idec,
    UD_Idiv,
    UD_Idivpd,
    UD_Idivps,
    UD_Idivsd,
    UD_Idivss,
    UD_Iemms,
    UD_Ienter,
    UD_If2xm1,
    UD_Ifabs,
    UD_Ifadd,
    UD_Ifaddp,
    UD_Ifbld,
    UD_Ifbstp,
    UD_Ifchs,
    UD_Ifclex,
    UD_Ifcmovb,
    UD_Ifcmove,
    UD_Ifcmovbe,
    UD_Ifcmovu,
    UD_Ifcmovnb,
    UD_Ifcmovne,
    UD_Ifcmovnbe,
    UD_Ifcmovnu,
    UD_Ifucomi,
    UD_Ifcom,
    UD_Ifcom2,
    UD_Ifcomp3,
    UD_Ifcomi,
    UD_Ifucomip,
    UD_Ifcomip,
    UD_Ifcomp,
    UD_Ifcomp5,
    UD_Ifcompp,
    UD_Ifcos,
    UD_Ifdecstp,
    UD_Ifdiv,
    UD_Ifdivp,
    UD_Ifdivr,
    UD_Ifdivrp,
    UD_Ifemms,
    UD_Iffree,
    UD_Iffreep,
    UD_Ificom,
    UD_Ificomp,
    UD_Ifild,
    UD_Ifincstp,
    UD_Ifninit,
    UD_Ifiadd,
    UD_Ifidivr,
    UD_Ifidiv,
    UD_Ifisub,
    UD_Ifisubr,
    UD_Ifist,
    UD_Ifistp,
    UD_Ifisttp,
    UD_Ifld,
    UD_Ifld1,
    UD_Ifldl2t,
    UD_Ifldl2e,
    UD_Ifldpi,
    UD_Ifldlg2,
    UD_Ifldln2,
    UD_Ifldz,
    UD_Ifldcw,
    UD_Ifldenv,
    UD_Ifmul,
    UD_Ifmulp,
    UD_Ifimul,
    UD_Ifnop,
    UD_Ifpatan,
    UD_Ifprem,
    UD_Ifprem1,
    UD_Ifptan,
    UD_Ifrndint,
    UD_Ifrstor,
    UD_Ifnsave,
    UD_Ifscale,
    UD_Ifsin,
    UD_Ifsincos,
    UD_Ifsqrt,
    UD_Ifstp,
    UD_Ifstp1,
    UD_Ifstp8,
    UD_Ifstp9,
    UD_Ifst,
    UD_Ifnstcw,
    UD_Ifnstenv,
    UD_Ifnstsw,
    UD_Ifsub,
    UD_Ifsubp,
    UD_Ifsubr,
    UD_Ifsubrp,
    UD_Iftst,
    UD_Ifucom,
    UD_Ifucomp,
    UD_Ifucompp,
    UD_Ifxam,
    UD_Ifxch,
    UD_Ifxch4,
    UD_Ifxch7,
    UD_Ifxrstor,
    UD_Ifxsave,
    UD_Ifxtract,
    UD_Ifyl2x,
    UD_Ifyl2xp1,
    UD_Ihlt,
    UD_Iidiv,
    UD_Iin,
    UD_Iimul,
    UD_Iinc,
    UD_Iinsb,
    UD_Iinsw,
    UD_Iinsd,
    UD_Iint1,
    UD_Iint3,
    UD_Iint,
    UD_Iinto,
    UD_Iinvd,
    UD_Iinvept,
    UD_Iinvlpg,
    UD_Iinvlpga,
    UD_Iinvvpid,
    UD_Iiretw,
    UD_Iiretd,
    UD_Iiretq,
    UD_Ijo,
    UD_Ijno,
    UD_Ijb,
    UD_Ijae,
    UD_Ijz,
    UD_Ijnz,
    UD_Ijbe,
    UD_Ija,
    UD_Ijs,
    UD_Ijns,
    UD_Ijp,
    UD_Ijnp,
    UD_Ijl,
    UD_Ijge,
    UD_Ijle,
    UD_Ijg,
    UD_Ijcxz,
    UD_Ijecxz,
    UD_Ijrcxz,
    UD_Ijmp,
    UD_Ilahf,
    UD_Ilar,
    UD_Ilddqu,
    UD_Ildmxcsr,
    UD_Ilds,
    UD_Ilea,
    UD_Iles,
    UD_Ilfs,
    UD_Ilgs,
    UD_Ilidt,
    UD_Ilss,
    UD_Ileave,
    UD_Ilfence,
    UD_Ilgdt,
    UD_Illdt,
    UD_Ilmsw,
    UD_Ilock,
    UD_Ilodsb,
    UD_Ilodsw,
    UD_Ilodsd,
    UD_Ilodsq,
    UD_Iloopne,
    UD_Iloope,
    UD_Iloop,
    UD_Ilsl,
    UD_Iltr,
    UD_Imaskmovq,
    UD_Imaxpd,
    UD_Imaxps,
    UD_Imaxsd,
    UD_Imaxss,
    UD_Imfence,
    UD_Iminpd,
    UD_Iminps,
    UD_Iminsd,
    UD_Iminss,
    UD_Imonitor,
    UD_Imontmul,
    UD_Imov,
    UD_Imovapd,
    UD_Imovaps,
    UD_Imovd,
    UD_Imovhpd,
    UD_Imovhps,
    UD_Imovlhps,
    UD_Imovlpd,
    UD_Imovlps,
    UD_Imovhlps,
    UD_Imovmskpd,
    UD_Imovmskps,
    UD_Imovntdq,
    UD_Imovnti,
    UD_Imovntpd,
    UD_Imovntps,
    UD_Imovntq,
    UD_Imovq,
    UD_Imovsb,
    UD_Imovsw,
    UD_Imovsd,
    UD_Imovsq,
    UD_Imovss,
    UD_Imovsx,
    UD_Imovupd,
    UD_Imovups,
    UD_Imovzx,
    UD_Imul,
    UD_Imulpd,
    UD_Imulps,
    UD_Imulsd,
    UD_Imulss,
    UD_Imwait,
    UD_Ineg,
    UD_Inop,
    UD_Inot,
    UD_Ior,
    UD_Iorpd,
    UD_Iorps,
    UD_Iout,
    UD_Ioutsb,
    UD_Ioutsw,
    UD_Ioutsd,
    UD_Ipacksswb,
    UD_Ipackssdw,
    UD_Ipackuswb,
    UD_Ipaddb,
    UD_Ipaddw,
    UD_Ipaddd,
    UD_Ipaddsb,
    UD_Ipaddsw,
    UD_Ipaddusb,
    UD_Ipaddusw,
    UD_Ipand,
    UD_Ipandn,
    UD_Ipavgb,
    UD_Ipavgw,
    UD_Ipcmpeqb,
    UD_Ipcmpeqw,
    UD_Ipcmpeqd,
    UD_Ipcmpgtb,
    UD_Ipcmpgtw,
    UD_Ipcmpgtd,
    UD_Ipextrb,
    UD_Ipextrd,
    UD_Ipextrq,
    UD_Ipextrw,
    UD_Ipinsrb,
    UD_Ipinsrw,
    UD_Ipinsrd,
    UD_Ipinsrq,
    UD_Ipmaddwd,
    UD_Ipmaxsw,
    UD_Ipmaxub,
    UD_Ipminsw,
    UD_Ipminub,
    UD_Ipmovmskb,
    UD_Ipmulhuw,
    UD_Ipmulhw,
    UD_Ipmullw,
    UD_Ipop,
    UD_Ipopa,
    UD_Ipopad,
    UD_Ipopfw,
    UD_Ipopfd,
    UD_Ipopfq,
    UD_Ipor,
    UD_Iprefetch,
    UD_Iprefetchnta,
    UD_Iprefetcht0,
    UD_Iprefetcht1,
    UD_Iprefetcht2,
    UD_Ipsadbw,
    UD_Ipshufw,
    UD_Ipsllw,
    UD_Ipslld,
    UD_Ipsllq,
    UD_Ipsraw,
    UD_Ipsrad,
    UD_Ipsrlw,
    UD_Ipsrld,
    UD_Ipsrlq,
    UD_Ipsubb,
    UD_Ipsubw,
    UD_Ipsubd,
    UD_Ipsubsb,
    UD_Ipsubsw,
    UD_Ipsubusb,
    UD_Ipsubusw,
    UD_Ipunpckhbw,
    UD_Ipunpckhwd,
    UD_Ipunpckhdq,
    UD_Ipunpcklbw,
    UD_Ipunpcklwd,
    UD_Ipunpckldq,
    UD_Ipi2fw,
    UD_Ipi2fd,
    UD_Ipf2iw,
    UD_Ipf2id,
    UD_Ipfnacc,
    UD_Ipfpnacc,
    UD_Ipfcmpge,
    UD_Ipfmin,
    UD_Ipfrcp,
    UD_Ipfrsqrt,
    UD_Ipfsub,
    UD_Ipfadd,
    UD_Ipfcmpgt,
    UD_Ipfmax,
    UD_Ipfrcpit1,
    UD_Ipfrsqit1,
    UD_Ipfsubr,
    UD_Ipfacc,
    UD_Ipfcmpeq,
    UD_Ipfmul,
    UD_Ipfrcpit2,
    UD_Ipmulhrw,
    UD_Ipswapd,
    UD_Ipavgusb,
    UD_Ipush,
    UD_Ipusha,
    UD_Ipushad,
    UD_Ipushfw,
    UD_Ipushfd,
    UD_Ipushfq,
    UD_Ipxor,
    UD_Ircl,
    UD_Ircr,
    UD_Irol,
    UD_Iror,
    UD_Ircpps,
    UD_Ircpss,
    UD_Irdmsr,
    UD_Irdpmc,
    UD_Irdtsc,
    UD_Irdtscp,
    UD_Irepne,
    UD_Irep,
    UD_Iret,
    UD_Iretf,
    UD_Irsm,
    UD_Irsqrtps,
    UD_Irsqrtss,
    UD_Isahf,
    UD_Isalc,
    UD_Isar,
    UD_Ishl,
    UD_Ishr,
    UD_Isbb,
    UD_Iscasb,
    UD_Iscasw,
    UD_Iscasd,
    UD_Iscasq,
    UD_Iseto,
    UD_Isetno,
    UD_Isetb,
    UD_Isetae,
    UD_Isetz,
    UD_Isetnz,
    UD_Isetbe,
    UD_Iseta,
    UD_Isets,
    UD_Isetns,
    UD_Isetp,
    UD_Isetnp,
    UD_Isetl,
    UD_Isetge,
    UD_Isetle,
    UD_Isetg,
    UD_Isfence,
    UD_Isgdt,
    UD_Ishld,
    UD_Ishrd,
    UD_Ishufpd,
    UD_Ishufps,
    UD_Isidt,
    UD_Isldt,
    UD_Ismsw,
    UD_Isqrtps,
    UD_Isqrtpd,
    UD_Isqrtsd,
    UD_Isqrtss,
    UD_Istc,
    UD_Istd,
    UD_Istgi,
    UD_Isti,
    UD_Iskinit,
    UD_Istmxcsr,
    UD_Istosb,
    UD_Istosw,
    UD_Istosd,
    UD_Istosq,
    UD_Istr,
    UD_Isub,
    UD_Isubpd,
    UD_Isubps,
    UD_Isubsd,
    UD_Isubss,
    UD_Iswapgs,
    UD_Isyscall,
    UD_Isysenter,
    UD_Isysexit,
    UD_Isysret,
    UD_Itest,
    UD_Iucomisd,
    UD_Iucomiss,
    UD_Iud2,
    UD_Iunpckhpd,
    UD_Iunpckhps,
    UD_Iunpcklps,
    UD_Iunpcklpd,
    UD_Iverr,
    UD_Iverw,
    UD_Ivmcall,
    UD_Ivmclear,
    UD_Ivmxon,
    UD_Ivmptrld,
    UD_Ivmptrst,
    UD_Ivmlaunch,
    UD_Ivmresume,
    UD_Ivmxoff,
    UD_Ivmread,
    UD_Ivmwrite,
    UD_Ivmrun,
    UD_Ivmmcall,
    UD_Ivmload,
    UD_Ivmsave,
    UD_Iwait,
    UD_Iwbinvd,
    UD_Iwrmsr,
    UD_Ixadd,
    UD_Ixchg,
    UD_Ixgetbv,
    UD_Ixlatb,
    UD_Ixor,
    UD_Ixorpd,
    UD_Ixorps,
    UD_Ixcryptecb,
    UD_Ixcryptcbc,
    UD_Ixcryptctr,
    UD_Ixcryptcfb,
    UD_Ixcryptofb,
    UD_Ixrstor,
    UD_Ixsave,
    UD_Ixsetbv,
    UD_Ixsha1,
    UD_Ixsha256,
    UD_Ixstore,
    UD_Iaesdec,
    UD_Iaesdeclast,
    UD_Iaesenc,
    UD_Iaesenclast,
    UD_Iaesimc,
    UD_Iaeskeygenassist,
    UD_Ipclmulqdq,
    UD_Igetsec,
    UD_Imovdqa,
    UD_Imaskmovdqu,
    UD_Imovdq2q,
    UD_Imovdqu,
    UD_Imovq2dq,
    UD_Ipaddq,
    UD_Ipsubq,
    UD_Ipmuludq,
    UD_Ipshufhw,
    UD_Ipshuflw,
    UD_Ipshufd,
    UD_Ipslldq,
    UD_Ipsrldq,
    UD_Ipunpckhqdq,
    UD_Ipunpcklqdq,
    UD_Iaddsubpd,
    UD_Iaddsubps,
    UD_Ihaddpd,
    UD_Ihaddps,
    UD_Ihsubpd,
    UD_Ihsubps,
    UD_Imovddup,
    UD_Imovshdup,
    UD_Imovsldup,
    UD_Ipabsb,
    UD_Ipabsw,
    UD_Ipabsd,
    UD_Ipshufb,
    UD_Iphaddw,
    UD_Iphaddd,
    UD_Iphaddsw,
    UD_Ipmaddubsw,
    UD_Iphsubw,
    UD_Iphsubd,
    UD_Iphsubsw,
    UD_Ipsignb,
    UD_Ipsignd,
    UD_Ipsignw,
    UD_Ipmulhrsw,
    UD_Ipalignr,
    UD_Ipblendvb,
    UD_Ipmuldq,
    UD_Ipminsb,
    UD_Ipminsd,
    UD_Ipminuw,
    UD_Ipminud,
    UD_Ipmaxsb,
    UD_Ipmaxsd,
    UD_Ipmaxud,
    UD_Ipmaxuw,
    UD_Ipmulld,
    UD_Iphminposuw,
    UD_Iroundps,
    UD_Iroundpd,
    UD_Iroundss,
    UD_Iroundsd,
    UD_Iblendpd,
    UD_Ipblendw,
    UD_Iblendps,
    UD_Iblendvpd,
    UD_Iblendvps,
    UD_Idpps,
    UD_Idppd,
    UD_Impsadbw,
    UD_Iextractps,
    UD_Iinsertps,
    UD_Imovntdqa,
    UD_Ipackusdw,
    UD_Ipmovsxbw,
    UD_Ipmovsxbd,
    UD_Ipmovsxbq,
    UD_Ipmovsxwd,
    UD_Ipmovsxwq,
    UD_Ipmovsxdq,
    UD_Ipmovzxbw,
    UD_Ipmovzxbd,
    UD_Ipmovzxbq,
    UD_Ipmovzxwd,
    UD_Ipmovzxwq,
    UD_Ipmovzxdq,
    UD_Ipcmpeqq,
    UD_Ipopcnt,
    UD_Iptest,
    UD_Ipcmpestri,
    UD_Ipcmpestrm,
    UD_Ipcmpgtq,
    UD_Ipcmpistri,
    UD_Ipcmpistrm,
    UD_Imovbe,
    UD_Icrc32,
    UD_MAX_MNEMONIC_CODE
} UD_ATTR_PACKED;

extern const char * ud_mnemonics_str[];

#endif /* UD_ITAB_H */
