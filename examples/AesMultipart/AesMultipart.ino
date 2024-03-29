#include "wsiotsdk.h"

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println();

  Serial.println("W3bstream SDK example: AES multi-part (streaming) encryption and decryption");
  Serial.println("This example shows how to use the W3bstream PSA Crypto API to encrypt and decrypt data using a multi-part (streaming) AES-CTR operation.");
  Serial.println("--------------------------------------------------------------------\n");

  // Define a variable status that will be used to store the result of the PSA API calls.
  psa_status_t status = PSA_SUCCESS;

  // The operation structure that will be used to store the encrypt operation context.
  psa_cipher_operation_t encryptOperation = PSA_CIPHER_OPERATION_INIT;

  // The operation structure that will be used to store the decrypt operation context.
  psa_cipher_operation_t decryptOperation = PSA_CIPHER_OPERATION_INIT;

  // The data to sign.
  uint8_t message[64] =
  {
    0xd8, 0x65, 0xc9, 0xcd, 0xea, 0x33, 0x56, 0xc5, 0x48, 0x8e, 0x7b, 0xa1, 0x5e, 0x84, 0xf4, 0xeb,
    0xa3, 0xb8, 0x25, 0x9c, 0x05, 0x3f, 0x24, 0xce, 0x29, 0x67, 0x22, 0x1c, 0x00, 0x38, 0x84, 0xd7,
    0x9d, 0x4c, 0xa4, 0x87, 0x7f, 0xfa, 0x4b, 0xc6, 0x87, 0xc6, 0x67, 0xe5, 0x49, 0x5b, 0xcf, 0xec,
    0x12, 0xf4, 0x87, 0x17, 0x32, 0xaa, 0xe4, 0x5a, 0x11, 0x06, 0x76, 0x11, 0x3d, 0xf9, 0xe7, 0xda
  };

  // Print the message.
  Serial.print("Encrypting ");
  Serial.print(sizeof(message));
  Serial.println(" bytes of data:");
  for (int i = 0; i < sizeof(message); i++)
  {
    if (message[i] < 0x10) Serial.print("0");
    Serial.print(message[i], HEX);
    Serial.print(" ");
    if ((i + 1) % 16 == 0) Serial.println();
  }
  Serial.println();

  // The key to use for the encryption and decryption.
  const uint8_t key[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

  // The AES initialization vector.
  const unsigned char iv[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

  // The buffer where the encrypted data will be stored. The size is the size of the message plus the size of the initialization vector (16 bytes).
  uint8_t encryptOutput[80] = {0};
  size_t encryptOutputLength = 0;
  size_t totalEncryptOutputLength = 0;

  // The buffer where the decrypted data will be stored.
  uint8_t decryptOutput[64] = {0};
  size_t decryptOutputLength = 0;
  size_t totalDecryptOutputLength = 0;

  // Initialise the library.
  psa_crypto_init();

  // Generate a random seed.
  SetEntropy(1);

  // A handle to the key managed by the library.
  psa_key_handle_t keyHandle = 0;

  // Initialise the key attributes structure.
  psa_key_attributes_t keyAttributes = PSA_KEY_ATTRIBUTES_INIT;
  psa_set_key_usage_flags(&keyAttributes, PSA_KEY_USAGE_ENCRYPT | PSA_KEY_USAGE_DECRYPT);
  psa_set_key_algorithm(&keyAttributes, PSA_ALG_CTR);
  psa_set_key_type(&keyAttributes, PSA_KEY_TYPE_AES);
  psa_set_key_bits(&keyAttributes, 128);

  // Import the key into the library and store the handle.
  status = psa_import_key(&keyAttributes, key, sizeof(key), &keyHandle);

  // Check that the key was imported successfully.
  if (status != PSA_SUCCESS)
  {
    Serial.println("Error: psa_import_key failed.");
    Serial.print("Status code: ");
    Serial.println(status);
    return;
  }

  // Start the encryption operation as a multi-part (streaming) operation.
  status = psa_cipher_encrypt_setup(&encryptOperation, keyHandle, PSA_ALG_CTR);
  if (status != PSA_SUCCESS)
  {
    Serial.println("Failed to setup encrypt operation.");
    Serial.print("Status code: ");
    Serial.println(status);
    return;
  }

  // Set the initialization vector.
  status = psa_cipher_set_iv(&encryptOperation, iv, 16);
  if (status != PSA_SUCCESS)
  {
    Serial.println("Failed to set iv.");
    Serial.print("Status code: ");
    Serial.println(status);
    return;
  }

  // Update the operation with the data to encrypt.
  status = psa_cipher_update(&encryptOperation, message, sizeof(message), encryptOutput, sizeof(encryptOutput), &encryptOutputLength);
  if (status != PSA_SUCCESS)
  {
    Serial.println("Failed to update operation.");
    Serial.print("Status code: ");
    Serial.println(status);
    return;
  }
  totalEncryptOutputLength += encryptOutputLength;

  // Finish the encryption operation.
  status = psa_cipher_finish(&encryptOperation, encryptOutput + encryptOutputLength, sizeof(encryptOutput) - encryptOutputLength, &encryptOutputLength);
  if (status != PSA_SUCCESS)
  {
    Serial.println("Failed to finish operation.");
    Serial.print("Status code: ");
    Serial.println(status);
    return;
  }
  totalEncryptOutputLength += encryptOutputLength;

  // Print the encrypted data.
  Serial.println("Encrypted data (" + String(totalEncryptOutputLength) + " bytes): ");
  for (int i = 0; i < totalEncryptOutputLength; i++)
  {
    if (encryptOutput[i] < 0x10) Serial.print("0");
    Serial.print(encryptOutput[i], HEX);
    Serial.print(" ");
    if ((i + 1) % 16 == 0) Serial.println();
  }
  Serial.println();

  // Start the decryption operation as a multi-part (streaming) operation.
  status = psa_cipher_decrypt_setup(&decryptOperation, keyHandle, PSA_ALG_CTR);
  if (status != PSA_SUCCESS)
  {
    Serial.println("Failed to setup decrypt operation.");
    Serial.print("Status code: ");
    Serial.println(status);
    return;
  }

  // Set the initialization vector.
  status = psa_cipher_set_iv(&decryptOperation, iv, 16);
  if (status != PSA_SUCCESS)
  {
    Serial.println("Failed to set iv.");
    Serial.print("Status code: ");
    Serial.println(status);
    return;
  }

  // Update the operation with the data to decrypt.
  status = psa_cipher_update(&decryptOperation, encryptOutput, totalEncryptOutputLength, decryptOutput, sizeof(decryptOutput), &decryptOutputLength);
  if (status != PSA_SUCCESS)
  {
    Serial.println("Failed to update operation.");
    Serial.print("Status code: ");
    Serial.println(status);
    return;
  }
  totalDecryptOutputLength += decryptOutputLength;

  // Finish the decryption operation.
  status = psa_cipher_finish(&decryptOperation, decryptOutput + decryptOutputLength, sizeof(decryptOutput) - decryptOutputLength, &decryptOutputLength);
  if (status != PSA_SUCCESS)
  {
    Serial.println("Failed to finish operation.");
    Serial.print("Status code: ");
    Serial.println(status);
    return;
  }
  totalDecryptOutputLength += decryptOutputLength;

  // Print the decrypted data.
  Serial.println("Decrypted data (" + String(totalDecryptOutputLength) + " bytes): ");
  for (int i = 0; i < totalDecryptOutputLength; i++)
  {
    if (decryptOutput[i] < 0x10) Serial.print("0");
    Serial.print(decryptOutput[i], HEX);
    Serial.print(" ");
    if ((i + 1) % 16 == 0) Serial.println();
  }
}

void loop()
{
  delay(1000);
}
