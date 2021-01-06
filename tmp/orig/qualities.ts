const qualities = {
  0: { nativeName: 'Auto' },
  240: { nativeName: '240p' },
  360: { nativeName: '360p' },
  480: { nativeName: '480p' },
  720: { nativeName: '720p' },
  1080: { nativeName: '1080p' },
  1440: { nativeName: '1440p' },
  4000: { nativeName: '4K' },
};

export type Qualities = keyof typeof qualities;

export default qualities;