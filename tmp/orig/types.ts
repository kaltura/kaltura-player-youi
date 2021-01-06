import { Languages } from './languages';

export interface BaseSettings {
  id: string;
  isSelected: boolean;
  label: string;
}

export interface LanguageSettings {
  language: Languages;
}

export interface Audio extends LanguageSettings, BaseSettings {
  bitrate: number;
  channelCount: number;
}

export interface Subtitle extends LanguageSettings, BaseSettings {}

export interface Video extends BaseSettings {
  height: number;
  width: number;
  isAdaptive: boolean;
  bitrate: number;
}

export interface Settings {
  audio: Array<Audio>;
  text: Array<Subtitle>;
  video: Array<Video>;
}

export interface MediaPlayout {
  mediaType: 'Vod' | 'Live';
}

export interface TimeUpdatePayload {
  position: number;
}