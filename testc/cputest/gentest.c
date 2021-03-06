#include <stdio.h>

extern void TEST_IMUL_R32_R32(unsigned int eax_edx_eflags[3],unsigned int eax,unsigned int edx);
extern void TEST_MUL_R32_R32(unsigned int eax_edx_eflags[3],unsigned int eax,unsigned int edx);

static unsigned int testNumberSrc16[]=
{
	0x0000,
	0x0001,
	0x0005,
	0x0010,
	0x0105,
	0x1000,
	0x7007,
	0x0100,
	0x0301,
	0x0705,
	0x0810,
	0x0A05,
	0x1C00,
	0x7E07,
	0x7FFF,
	0x8000,
	0x8707,
	0x8000,
	0x8FFF,
	0x800B,
	0xC00B,
	0xB000,
	0xA0BD,
	0xFFFF,
};
static unsigned int testNumberSrc32[]=
{
	0x00000000,
	0x00000001,
	0x00000005,
	0x00000010,
	0x00000105,
	0x00001000,
	0x00007007,
	0x00010000,
	0x00100055,
	0x01000000,
	0x10000707,
	0x40000000,
	0x7FFFFFFF,
	0x80000000,
	0x800B000B,
	0xA0000000,
	0xA00B000B,
	0xB00B000B,
	0xD00B000B,
	0xF0000000,
	0xFFFFFFFF,
};
static unsigned int testNumberSrc8[]=
{
	0x00,
	0x01,
	0x03,
	0x07,
	0x10,
	0x21,
	0x47,
	0x7F,
	0x80,
	0xB0,
	0xC0,
	0xD0,
	0xF0,
	0xFF,
};

#define LEN(x) (sizeof(x)/sizeof(x[0]))

void GenImulR32xR32Test(FILE *ofp)
{
	fprintf(ofp,"unsigned int IMUL_32_32_TABLE[]={\n");
	for(int i=0; i<LEN(testNumberSrc32); ++i)
	{
		for(int j=0; j<LEN(testNumberSrc32); ++j)
		{
			unsigned int eax=testNumberSrc32[i];
			unsigned int edx=testNumberSrc32[j];
			unsigned int eax_edx_eflags[3];
			TEST_IMUL_R32_R32(eax_edx_eflags,eax,edx);
			fprintf(ofp,"\t0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,\n",eax,edx,eax_edx_eflags[0],eax_edx_eflags[1],eax_edx_eflags[2]);
		}
	}
	fprintf(ofp,"};\n");
}

void GenMulR32xR32Test(FILE *ofp)
{
	fprintf(ofp,"unsigned int MUL_32_32_TABLE[]={\n");
	for(int i=0; i<LEN(testNumberSrc32); ++i)
	{
		for(int j=0; j<LEN(testNumberSrc32); ++j)
		{
			unsigned int eax=testNumberSrc32[i];
			unsigned int edx=testNumberSrc32[j];
			unsigned int eax_edx_eflags[3];
			TEST_MUL_R32_R32(eax_edx_eflags,eax,edx);
			fprintf(ofp,"\t0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,\n",eax,edx,eax_edx_eflags[0],eax_edx_eflags[1],eax_edx_eflags[2]);
		}
	}
	fprintf(ofp,"};\n");
}

extern void TEST_IMUL_R32_R32_I8(unsigned int res[32],unsigned int EAX,unsigned int EDX);

void GetnImulR32xR32xI8Test(FILE *ofp)
{
	unsigned int res[32];
	fprintf(ofp,"unsigned int IMUL_32_32_I8_TABLE[]={\n");
	for(int i=0; i<LEN(testNumberSrc32); ++i)
	{
		TEST_IMUL_R32_R32_I8(res,i,testNumberSrc32[i]);
		fprintf(ofp,"\t0x%08x,0x%08x,\n",i,testNumberSrc32[i]);
		for(int j=0; j<32; ++j)
		{
			if(0==(j%16))
			{
				fprintf(ofp,"\t");
			}
			fprintf(ofp,"0x%08x,",res[j]);
			if(15==(j%16))
			{
				fprintf(ofp,"\n");
			}
		}
	}
	fprintf(ofp,"};\n");
}

extern C0_BITSHIFT_R8_I8(unsigned int res[16],unsigned int v0);
extern C1_BITSHIFT_R_I8(unsigned int res[16],unsigned int v0);
extern D3_BITSHIFT_R_CL(unsigned int res[16],unsigned int v0);

void GenBitShift(FILE *ofp)
{
	unsigned int res[128];
	{
		fprintf(ofp,"unsigned int BitShiftR8_I8_TABLE[]={\n");
		for(int i=0; i<LEN(testNumberSrc8); ++i)
		{
			unsigned int eax=testNumberSrc8[i];
			C0_BITSHIFT_R8_I8(res,eax);
			fprintf(ofp,"\t0x%08x,\n",eax);
			for(int j=0; j<128; ++j)
			{
				if(0==(j%16))
				{
					fprintf(ofp,"\t");
				}
				fprintf(ofp,"0x%08x,",res[j]);
				if(15==(j%16))
				{
					fprintf(ofp,"\n");
				}
			}
		}
		fprintf(ofp,"};\n");
	}
	{
		fprintf(ofp,"unsigned int BitShiftR_I8_TABLE[]={\n");
		for(int i=0; i<LEN(testNumberSrc32); ++i)
		{
			unsigned int eax=testNumberSrc32[i];
			C1_BITSHIFT_R_I8(res,eax);
			fprintf(ofp,"\t0x%08x,\n",eax);
			for(int j=0; j<128; ++j)
			{
				if(0==(j%16))
				{
					fprintf(ofp,"\t");
				}
				fprintf(ofp,"0x%08x,",res[j]);
				if(15==(j%16))
				{
					fprintf(ofp,"\n");
				}
			}
		}
		fprintf(ofp,"};\n");
	}
	{
		fprintf(ofp,"unsigned int BitShiftR_CL_TABLE[]={\n");
		for(int i=0; i<LEN(testNumberSrc32); ++i)
		{
			unsigned int eax=testNumberSrc32[i];
			D3_BITSHIFT_R_CL(res,eax);
			fprintf(ofp,"\t0x%08x,\n",eax);
			for(int j=0; j<128; ++j)
			{
				if(0==(j%16))
				{
					fprintf(ofp,"\t");
				}
				fprintf(ofp,"0x%08x,",res[j]);
				if(15==(j%16))
				{
					fprintf(ofp,"\n");
				}
			}
		}
		fprintf(ofp,"};\n");
	}
}

extern void TEST_F6(unsigned int res[],unsigned int eax,unsigned int edx);
extern void TEST_F7(unsigned int res[],unsigned int eax,unsigned int edx);

void GenF6F7_NOT_NEG_MUL_IMUL_DIV_IDIV(FILE *ofp)
{
	int i;
	fprintf(ofp,"unsigned int F6_8_8_TABLE[]={\n");
	for(i=0; i<LEN(testNumberSrc8); ++i)
	{
		for(int j=0; j<LEN(testNumberSrc8); ++j)
		{
			int k;
			unsigned int eax=testNumberSrc8[i];
			unsigned int edx=testNumberSrc8[j];
			unsigned int res[12];
			TEST_F6(res,eax,edx);
			fprintf(ofp,"\t0x%08x,0x%08x,\n",eax,edx);
			fprintf(ofp,"\t");
			for(k=0; k<6; ++k)
			{
				fprintf(ofp,"0x%08x,",res[k]);
			}
			fprintf(ofp,"\n");
			fprintf(ofp,"\t");
			for(k=6; k<12; ++k)
			{
				fprintf(ofp,"0x%08x,",res[k]);
			}
			fprintf(ofp,"\n");
		}
	}
	fprintf(ofp,"};\n");

	fprintf(ofp,"unsigned int F7_32_32_TABLE[]={\n");
	for(i=0; i<LEN(testNumberSrc32); ++i)
	{
		for(int j=0; j<LEN(testNumberSrc32); ++j)
		{
			int k;
			unsigned int eax=testNumberSrc8[i];
			unsigned int edx=testNumberSrc8[j];
			unsigned int res[12];
			TEST_F7(res,eax,edx);
			fprintf(ofp,"\t0x%08x,0x%08x,\n",eax,edx);
			fprintf(ofp,"\t");
			for(k=0; k<6; ++k)
			{
				fprintf(ofp,"0x%08x,",res[k]);
			}
			fprintf(ofp,"\n");
			fprintf(ofp,"\t");
			for(k=6; k<12; ++k)
			{
				fprintf(ofp,"0x%08x,",res[k]);
			}
			fprintf(ofp,"\n");
		}
	}
	fprintf(ofp,"};\n");

}

extern void TEST_R8_I8(unsigned int res[16],unsigned int ebx);
extern void TEST_R32_I32(unsigned int res[16],unsigned int ebx);

void GenF6F7_TEST_R_I(FILE *ofp)
{
	int i;
	fprintf(ofp,"unsigned int F6_TEST_I8_TABLE[]={\n");
	for(i=0; i<LEN(testNumberSrc8); ++i)
	{
		unsigned int res[16];
		TEST_R8_I8(res,testNumberSrc8[i]);
		fprintf(ofp,"\t0x%08x,\n",testNumberSrc8[i]);
		for(int j=0; j<16; ++j)
		{
			if(0==(j%8))
			{
				fprintf(ofp,"\t");
			}
			fprintf(ofp,"0x%08x,",res[j]);
			if(7==(j%8))
			{
				fprintf(ofp,"\n");
			}
		}
	}
	fprintf(ofp,"};\n");

	fprintf(ofp,"unsigned int F7_TEST_I32_TABLE[]={\n");
	for(i=0; i<LEN(testNumberSrc32); ++i)
	{
		unsigned int res[16];
		TEST_R32_I32(res,testNumberSrc32[i]);
		fprintf(ofp,"\t0x%08x,\n",testNumberSrc32[i]);
		for(int j=0; j<16; ++j)
		{
			if(0==(j%8))
			{
				fprintf(ofp,"\t");
			}
			fprintf(ofp,"0x%08x,",res[j]);
			if(7==(j%8))
			{
				fprintf(ofp,"\n");
			}
		}
	}
	fprintf(ofp,"};\n");
}

extern void TEST_AAD(unsigned int res[],unsigned int eax);
extern void TEST_AAM(unsigned int res[],unsigned int eax);
extern void TEST_AAS(unsigned int res[],unsigned int eax);

void GenAADAAMAAS(FILE *ofp)
{
	int i,j;
	fprintf(ofp,"unsigned int AAD_TABLE[]={\n");
	for(i=0; i<16; ++i)
	{
		for(j=0; j<16; ++j)
		{
			unsigned int res[2];
			unsigned int eax=i*256+j;
			TEST_AAD(res,eax);
			fprintf(ofp,"\t0x%08x,0x%08x,0x%08x,\n",eax,res[0],res[1]);
		}
	}
	fprintf(ofp,"};\n");

	fprintf(ofp,"unsigned int AAM_TABLE[]={\n");
	for(i=0; i<16; ++i)
	{
		for(j=0; j<16; ++j)
		{
			unsigned int res[2];
			unsigned int eax=i*16+j;
			TEST_AAM(res,eax);
			fprintf(ofp,"\t0x%08x,0x%08x,0x%08x,\n",eax,res[0],res[1]);
		}
	}
	fprintf(ofp,"};\n");

	fprintf(ofp,"unsigned int AAS_TABLE[]={\n");
	for(i=0; i<16; ++i)
	{
		for(j=0; j<16; ++j)
		{
			unsigned int res[2];
			unsigned int eax=i*16+j;
			TEST_AAS(res,eax);
			fprintf(ofp,"\t0x%08x,0x%08x,0x%08x,\n",eax,res[0],res[1]);
		}
	}
	fprintf(ofp,"};\n");
}

extern void BTX_R32_I8(unsigned int res[],unsigned int EBX);
extern void BTX_R32_R32(unsigned int res[],unsigned int EBX,unsigned int ECX);

void GenBTx(FILE *ofp)
{
	int i;
	unsigned int res[64];
	fprintf(ofp,"unsigned int BTX_I8_TABLE[]={\n");
	for(i=0; i<LEN(testNumberSrc32); ++i)
	{
		BTX_R32_I8(res,testNumberSrc32[i]);
		fprintf(ofp,"\t0x%08x,\n",testNumberSrc32[i]);
		for(int j=0; j<64; ++j)
		{
			if(0==(j%8))
			{
				fprintf(ofp,"\t");
			}
			fprintf(ofp,"0x%08x,",res[j]);
			if(7==(j%8))
			{
				fprintf(ofp,"\n");
			}
		}
	}
	fprintf(ofp,"};\n");

	fprintf(ofp,"unsigned int BTX_R32_R32_TABLE[]={\n");
	for(i=0; i<LEN(testNumberSrc32); ++i)
	{
		for(int j=0; j<LEN(testNumberSrc32); ++j)
		{
			BTX_R32_R32(res,testNumberSrc32[i],testNumberSrc32[j]);
			fprintf(ofp,"\t0x%08x,0x%08x,\n",testNumberSrc32[i],testNumberSrc32[j]);
			for(int j=0; j<8; ++j)
			{
				if(0==(j%8))
				{
					fprintf(ofp,"\t");
				}
				fprintf(ofp,"0x%08x,",res[j]);
				if(7==(j%8))
				{
					fprintf(ofp,"\n");
				}
			}
		}
	}
	fprintf(ofp,"};\n");
}

extern void TEST_CBW(unsigned int *res,unsigned int eax);
extern void TEST_CWDE(unsigned int *res,unsigned int eax);
extern void TEST_CWD(unsigned int res[2],unsigned int eax,unsigned int edx);
extern void TEST_CDQ(unsigned int res[2],unsigned int eax,unsigned int edx);

void GenCBW_CWDE_CWD_CDQ(FILE *ofp)
{
	int i;
	unsigned int res[2];
	fprintf(ofp,"unsigned int CBW_TABLE[]={\n");
	for(i=0; i<256; ++i)
	{
		TEST_CBW(res,i);
		fprintf(ofp,"\t0x%08x,0x%08x,\n",i,res[0]);
	}
	fprintf(ofp,"};\n");

	fprintf(ofp,"unsigned int CWDE_TABLE[]={\n");
	for(i=0; i<LEN(testNumberSrc16); ++i)
	{
		TEST_CWDE(res,testNumberSrc16[i]);
		fprintf(ofp,"\t0x%08x,0x%08x,\n",testNumberSrc16[i],res[0]);
	}
	fprintf(ofp,"};\n");

	fprintf(ofp,"unsigned int CWD_TABLE[]={\n");
	for(i=0; i<LEN(testNumberSrc16); ++i)
	{
		TEST_CWD(res,testNumberSrc16[i],testNumberSrc16[i]);
		fprintf(ofp,"\t0x%08x,0x%08x,0x%08x,\n",testNumberSrc16[i],res[0],res[1]);
	}
	fprintf(ofp,"};\n");
	fprintf(ofp,"unsigned int CDQ_TABLE[]={\n");
	for(i=0; i<LEN(testNumberSrc32); ++i)
	{
		TEST_CDQ(res,testNumberSrc32[i],testNumberSrc32[i]);
		fprintf(ofp,"\t0x%08x,0x%08x,0x%08x,\n",testNumberSrc32[i],res[0],res[1]);
	}
	fprintf(ofp,"};\n");
}

int main(void)
{
	FILE *ofp=fopen("cputest/testcase.h","w");
	GenImulR32xR32Test(ofp);
	GenMulR32xR32Test(ofp);
	GetnImulR32xR32xI8Test(ofp);
	GenBitShift(ofp);
	GenF6F7_NOT_NEG_MUL_IMUL_DIV_IDIV(ofp);
	GenF6F7_TEST_R_I(ofp);
	GenAADAAMAAS(ofp);
	GenBTx(ofp);
	GenCBW_CWDE_CWD_CDQ(ofp);
	fclose(ofp);
	return 0;
}
