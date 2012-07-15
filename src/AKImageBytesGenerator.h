#ifndef KinectExtension_AKImageBytesGenerator_h
#define KinectExtension_AKImageBytesGenerator_h

class AKImageBytesGenerator
{
public:
	AKImageBytesGenerator();
	~AKImageBytesGenerator();

	void setSourceSize(int width, int height);
	void setTargetSize(int width, int height);

	bool getSourceMirrored();
	void setSourceMirrored(bool value);

	bool getTargetMirrored();
	void setTargetMirrored(bool value);

	int getSourceWidth();
	int getSourceHeight();
	int getSourcePixelCount();
	int getTargetWidth();
	int getTargetHeight();
	int getTargetPixelCount();
	int getScale();

	void setSourceBytes(unsigned int* sourceBytes);
	unsigned int* getTargetBytes();

	void generateTargetBytes();


private:
	int _sourceWidth;
	int _sourceHeight;
	int _sourcePixelCount;
	bool _sourceMirrored;
	int _targetPixelCount;
	int _targetWidth;
	int _targetHeight;
	bool _targetMirrored;
	int _scale;

	int _direction;
	int _directionFactor;
	int _scaledWidth;

	unsigned int* _sourceBytes;
	unsigned int* _targetBytes;

	void generateTargetBytesForRow(int row);

	void updateScale();
	void updateTargetBytesLength();
	void updateTargetBytesDirection();
};

#endif