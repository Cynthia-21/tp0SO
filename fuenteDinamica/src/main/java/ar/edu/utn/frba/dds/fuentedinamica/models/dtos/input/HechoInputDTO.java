package ar.edu.utn.frba.dds.fuentedinamica.models.dtos.input;

import lombok.Data;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.util.List;

@Data
public class HechoInputDTO {
  private String titulo;
  private String descripcion;
  private List<String> categorias;
  private String contenidoMultimedia;
  private String ubicacion;
  private LocalDate fechaAcontecimiento;
  private LocalDateTime fechaDeCarga;
  private String origen;
  private String contribuyente;
  private List<String> etiquetas;

}