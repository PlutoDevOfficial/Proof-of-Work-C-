#include <openssl/evp.h>
#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

std::string sha256(const std::string& input) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;
    
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    
    if (!context) {
        std::cerr << "Failed to create EVP context" << std::endl;
        return "";
    }
    
    if (1 != EVP_DigestInit_ex(context, EVP_sha256(), NULL)) {
        std::cerr << "Failed to initialize digest" << std::endl;
        EVP_MD_CTX_free(context);
        return "";
    }
    
    if (1 != EVP_DigestUpdate(context, input.c_str(), input.length())) {
        std::cerr << "Failed to update digest" << std::endl;
        EVP_MD_CTX_free(context);
        return "";
    }
    
    if (1 != EVP_DigestFinal_ex(context, hash, &lengthOfHash)) {
        std::cerr << "Failed to finalize digest" << std::endl;
        EVP_MD_CTX_free(context);
        return "";
    }
    
    EVP_MD_CTX_free(context);
    
    std::stringstream ss;
    for (unsigned int i = 0; i < lengthOfHash; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    
    return ss.str();
}

bool proofOfWork(const std::string& challenge, int difficulty) {
    unsigned long nonce = 0;
    std::string hashResult;
    std::string target(difficulty, '0');
    
    do {
        nonce++;
        std::string input = challenge + std::to_string(nonce);
        hashResult = sha256(input);
    } while (hashResult.substr(0, difficulty) != target);

    std::cout << "Solution found! Nonce: " << nonce << " Hash: " << hashResult << std::endl;
    return true;
}

int main() {
    std::string challenge = "SomeBullshitString";
    int difficulty = 5; // Adjust difficulty based on system performance
    proofOfWork(challenge, difficulty);
    return 0;
}
