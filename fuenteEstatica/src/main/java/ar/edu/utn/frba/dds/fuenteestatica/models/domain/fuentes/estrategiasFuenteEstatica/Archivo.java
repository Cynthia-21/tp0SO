package ar.edu.utn.frba.dds.fuenteestatica.models.domain.fuentes.estrategiasFuenteEstatica;

import lombok.AllArgsConstructor;
import lombok.Getter;

@Getter@AllArgsConstructor
public class Archivo {
  private String nombre;
  private Integer tamanioEnBytes;
}
