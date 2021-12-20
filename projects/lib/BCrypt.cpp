
#include "pch.h"
#include "IBinary.h"
#include <bcrypt.h>

#pragma comment(lib, "bcrypt.lib") // over VISTA

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) (((NTSTATUS)Status) >= 0)
#endif

#define ENCRYPT_BLOCK_SIZE 16 // aes block size

#define INIT_VECTOR {0x0a, 0x01, 0x08, 0x03, 0x04, 0x0d, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x03}


void IStreamSeekToTop(IStream* sm )
{
	ULARGE_INTEGER len;
	LARGE_INTEGER s={};

    sm->Seek(s,STREAM_SEEK_SET,&len);
}
IStream* ToIStream( const IBinary& src )
{
	ComPTR<IStream> is;
	auto r = CreateStreamOnHGlobal(NULL,TRUE,&is );
	
	DWORD dw;
	is->Write(IBinaryPtr(src), IBinaryLen(src), &dw );
	
	IStreamSeekToTop(is);
	is.p->AddRef();

	return is;
}
DWORD IStreamLength(IStream* sm )
{
	ULARGE_INTEGER len;
	LARGE_INTEGER s={};

    sm->Seek(s,STREAM_SEEK_END,&len);

	return (DWORD)(0xFFFFFFFF & len.QuadPart);
}
IBinary ToIBinary( IStream* bin )
{
	DWORD cblen = IStreamLength(bin);
	IStreamSeekToTop( bin );
	
	BYTE* cb = new BYTE[cblen];
	ULONG read;

	bin->Read(cb, cblen, &read);
	return IBinary(IBinaryMk(cb, read));
}



bool AES( bool IsEncrypt, byte* pwd, int pwdlen, IStream* src, IStream** dst )
{
	// CNG アルゴリズムプロバイ
	bool ret = false;
	BCRYPT_ALG_HANDLE hAlgorithm = nullptr;
	BCRYPT_KEY_HANDLE hKey = nullptr;
	DWORD             dwKeyObjectSize;
	LPBYTE            lpKeyObject = nullptr;
	LPBYTE            lpData = nullptr;
	DWORD             dwDataSize;
	DWORD             dwResult;
	NTSTATUS          status;

	_ASSERT( pwdlen == 32 ); // 32:AES256 , 16:AES128

	
	unsigned long IVLength;

	status = BCryptOpenAlgorithmProvider(&hAlgorithm, BCRYPT_AES_ALGORITHM, nullptr, 0);

	status = BCryptGetProperty(hAlgorithm, BCRYPT_OBJECT_LENGTH, (LPBYTE)&dwKeyObjectSize, sizeof(DWORD), &dwResult, 0);

	status = BCryptGetProperty(hAlgorithm, BCRYPT_BLOCK_LENGTH, (unsigned char*)(&IVLength), sizeof(IVLength), &dwResult, 0);

	BCRYPT_KEY_LENGTHS_STRUCT KeyLength = {0};
	status = BCryptGetProperty(hAlgorithm, BCRYPT_KEY_LENGTHS, (unsigned char*)(&KeyLength), sizeof(KeyLength), &dwResult,0);

	LPBYTE pKeyObject = new byte[dwKeyObjectSize];
	LPBYTE pIV = new byte[]INIT_VECTOR;

	lpKeyObject = new byte[dwKeyObjectSize];

	status = BCryptSetProperty(hAlgorithm, BCRYPT_CHAINING_MODE,(PBYTE)BCRYPT_CHAIN_MODE_CBC,sizeof(BCRYPT_CHAIN_MODE_CBC), 0);
	if ( !NT_SUCCESS(status))
		goto err;

	status = BCryptGenerateSymmetricKey(hAlgorithm, &hKey, pKeyObject, dwKeyObjectSize, pwd, pwdlen,0 ); 

	if ( NT_SUCCESS(status))
	{
		if (IsEncrypt)
		{
			IBinary bins = ToIBinary(src);
			lpData = IBinaryPtr(bins);
			dwDataSize = IBinaryLen(bins);
		
			DWORD  dwEncryptDataSize = ((dwDataSize - 1) / ENCRYPT_BLOCK_SIZE + 1) * ENCRYPT_BLOCK_SIZE;
			LPBYTE lpEncryptData = new byte[dwEncryptDataSize];
				
			status = BCryptEncrypt(hKey, lpData, dwDataSize, NULL, pIV, IVLength, lpEncryptData, dwEncryptDataSize, &dwEncryptDataSize, BCRYPT_BLOCK_PADDING|BCRYPT_PAD_PKCS1);

			if (NT_SUCCESS(status))
			{
				IBinary encryptBin(IBinaryMk(lpEncryptData, dwEncryptDataSize));
	
				*dst = ToIStream(encryptBin);			
				ret = true;
			}
			else
				delete [] lpEncryptData;
		}
		else
		{			
			LPBYTE lpEncryptData;
			DWORD  dwEncryptDataSize;

			LPBYTE lpData;
			DWORD  dwDataSize, dwDecryptDataSize;

			IBinary bins = ToIBinary(src);
			lpEncryptData = IBinaryPtr(bins);
			dwEncryptDataSize = IBinaryLen(bins);
			dwDataSize = dwEncryptDataSize;
	
			lpData = new byte[dwEncryptDataSize];
			status = BCryptDecrypt(hKey, lpEncryptData, dwEncryptDataSize, NULL, pIV, IVLength, lpData, dwDataSize, &dwDecryptDataSize, BCRYPT_BLOCK_PADDING|BCRYPT_PAD_PKCS1);

			if (NT_SUCCESS(status)) 
			{
				IBinary encryptBin(IBinaryMk(lpData, dwDecryptDataSize));
				*dst = ToIStream(encryptBin);
				ret = true;
			
			}	
			else
			{
				DWORD xx = status;
				if (0xC000003EL == xx )
				{
					int a = 0;
				}

				delete[] lpData;
			}
		}
	}
err:	
	delete[] lpKeyObject;
	delete[] pIV;
	BCryptDestroyKey(hKey);
	BCryptCloseAlgorithmProvider(hAlgorithm, 0);

	
	return ret;
}

/*
public static byte[] Encrypt(byte[] data, byte[] key1, byte[] iv)
		{
			byte[] key = key1;

			AesCryptoServiceProvider aes = new AesCryptoServiceProvider();

			aes.Mode = CipherMode.CBC;
			aes.Padding = PaddingMode.PKCS7;
			aes.KeySize = 256;
			aes.BlockSize = 128;
			

			var decryptor = aes.CreateEncryptor(key, iv);
			byte[] input = data;

			using (MemoryStream msDecrypt = new MemoryStream())
			{
				using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Write))
				{
					csDecrypt.Write(input, 0, input.Length);
					csDecrypt.FlushFinalBlock();
				}

				return msDecrypt.ToArray();
			}
		}
		public static byte[] Decrypt(byte[] input, byte[] key, byte[] iv)
		{
			try
			{
				AesCryptoServiceProvider aes = new AesCryptoServiceProvider(); // AES
				
				aes.Mode = System.Security.Cryptography.CipherMode.CBC;
				aes.Padding = System.Security.Cryptography.PaddingMode.PKCS7;
				aes.KeySize = 256; 
				aes.BlockSize = 128;

				var decryptor = aes.CreateDecryptor(key, iv);

				using (MemoryStream msDecrypt = new MemoryStream())
				{
					using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Write))
					{
						csDecrypt.Write(input, 0, input.Length);
						csDecrypt.FlushFinalBlock();
					}

					return msDecrypt.ToArray();
				}
			}
			catch (Exception ex)
			{

				return null;
			}
		}
*/